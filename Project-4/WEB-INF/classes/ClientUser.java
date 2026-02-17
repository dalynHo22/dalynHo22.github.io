/* Name: Dalyn Ho 
Course: CNT 4714 – Fall 2025 – Project Four
Assignment title: A Three-Tier Distributed Web-Based Application
Date: December 1, 2025
Class: Enterprise Computing*/

import java.io.*;
import java.sql.*;
import java.util.Properties;

import jakarta.servlet.*;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.*;

@WebServlet("/ClientUser")
public class ClientUser extends HttpServlet {

    private Connection conn;

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws IOException, ServletException {

        String sqlStatement = request.getParameter("sql");
        String lowerCaseStatement = sqlStatement != null ? sqlStatement.toLowerCase().trim() : "";

        String finalMess = "";

        try {
            // Load database properties
            Properties dbProperties = new Properties();
            ServletContext context = getServletContext();

            // client.properties lives in WEB-INF/conf
            try (InputStream inputStream = context.getResourceAsStream("/WEB-INF/conf/client.properties")) {
                if (inputStream == null) {
                    finalMess =
                        "<table><tr bgcolor='red'><th style='text-align:center; background-color: red;'>" +
                        "<font color='#ffffff'><b>" +
                        "Properties file not found: /WEB-INF/conf/client.properties" +
                        "</b></font></th></tr></table>";
                } else {
                    dbProperties.load(inputStream);

                    Class.forName(dbProperties.getProperty("MYSQL_DB_DRIVER_CLASS"));
                    conn = DriverManager.getConnection(
                            dbProperties.getProperty("MYSQL_DB_URL"),
                            dbProperties.getProperty("MYSQL_DB_USERNAME"),
                            dbProperties.getProperty("MYSQL_DB_PASSWORD"));

                    // ---- MAIN CLIENT LOGIC ----

                    if (sqlStatement == null || lowerCaseStatement.isEmpty()) {
                        finalMess =
                            "<table><tr bgcolor='red'><th style='text-align:center; background-color: red;'>" +
                            "<font color='#ffffff'><b>No SQL statement entered.</b></font></th></tr></table>";
                    }
                    else if (lowerCaseStatement.startsWith("select")) {
                        // SELECT queries are allowed
                        try (Statement statement = conn.createStatement();
                             ResultSet resultSet = statement.executeQuery(sqlStatement)) {
                            finalMess = outputTable.getHtml(resultSet);
                        }
                    } else {
                        // Any non-SELECT should be blocked for client user
                        finalMess =
                            "<table><tr bgcolor='red'><th style='text-align:center; background-color: red;'>" +
                            "<font color='#ffffff'><b>" +
                            "Client-level users are only allowed to execute SELECT statements. " +
                            "No changes were made to the database." +
                            "</b></font></th></tr></table>";
                    }
                }
            } catch (IOException | ClassNotFoundException | SQLException e) {
                finalMess =
                    "<table><tr bgcolor='red'><th style='text-align:center; background-color: red;'>" +
                    "<font color='#ffffff'><b>" +
                    "Database connection failed: " + e.getMessage() +
                    "</b></font></th></tr></table>";
            }
        } catch (Exception e) {
            finalMess =
                "<table><tr bgcolor='red'><th style='text-align:center; width: fit-content; background-color: red;'>" +
                "<font color='#ffffff'><b>" +
                "Error executing the SQL Statement: " + e.getMessage() +
                "</b></font></th></tr></table>";
        } finally {
            // Close database connection
            if (conn != null) {
                try {
                    conn.close();
                } catch (SQLException e) {
                    // just log; don't overwrite the main message
                    System.err.println("Error closing client DB connection: " + e.getMessage());
                }
            }
        }

        // Put results & original SQL in the session so clientHome.jsp can show them
        HttpSession session = request.getSession();
        session.setAttribute("tableResults", finalMess);
        session.setAttribute("sql", sqlStatement);

        // Go back to clientHome.jsp
        response.sendRedirect("clientHome.jsp");
    }

    // Same outputTable helper as your RootUser
    public static class outputTable {
        public static synchronized String getHtml(ResultSet rs) throws SQLException {
            if (rs == null) {
                return "<table><tr><td>No results found.</td></tr></table>";
            }

            ResultSetMetaData metaData = rs.getMetaData();
            int colCount = metaData.getColumnCount();
            StringBuilder html = new StringBuilder();
            
            html.append("<table border='1' style='border-collapse: collapse; width: fit-content;'>");
            html.append("<tr style='background-color: #f2f2f2;'>");

            // Add column headers
            for (int i = 1; i <= colCount; i++) {
                String columnName = metaData.getColumnName(i);
                html.append("<th style='padding: 8px; text-align: left;'>")
                    .append(columnName)
                    .append("</th>");
            }
            html.append("</tr>");

            // Add rows from ResultSet
            while (rs.next()) {
                html.append("<tr>");
                for (int i = 1; i <= colCount; i++) {
                    String cellValue = rs.getString(i);
                    html.append("<td style='padding: 8px;'>")
                        .append(cellValue != null ? cellValue : "")
                        .append("</td>");
                }
                html.append("</tr>");
            }

            html.append("</table>");
            return html.toString();
        }
    }
}

