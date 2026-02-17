/* Name: Dalyn Ho 
Course: CNT 4714 – Fall 2025 – Project Four
Assignment title: A Three-Tier Distributed Web-Based Application
Date: December 1, 2025
Class: Enterprise Computing*/

import jakarta.servlet.*;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.*;
import java.io.*;
import java.sql.*;
import java.util.Properties;

@WebServlet("/AccountantUser")
public class AccountantUser extends HttpServlet {
    private Connection connection;

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String reportType = request.getParameter("query");
        String htmlResult = "";

        try {
            establishDatabaseConnection();
            htmlResult = executeStoredProcedure(reportType);

        } catch (SQLException e) {
            htmlResult = "<h3 style='color:red;'>Error: Unable to process your request. " 
                        + e.getMessage() + "</h3>";
            e.printStackTrace();
        } finally {
            closeDatabaseConnection();
        }

        HttpSession session = request.getSession();
        session.setAttribute("tableResults", htmlResult);
        response.sendRedirect("accountantHome.jsp");
    }

    private void establishDatabaseConnection() throws SQLException {
        Properties dbProperties = new Properties();
        ServletContext context = getServletContext();

        // FIXED PATH HERE
        try (InputStream inputStream = context.getResourceAsStream("/WEB-INF/conf/accountant.properties")) {
            if (inputStream == null) {
                throw new IOException("Properties file not found: /WEB-INF/conf/accountant.properties");
            }

            dbProperties.load(inputStream);

            String dbDriver = dbProperties.getProperty("MYSQL_DB_DRIVER_CLASS");
            String dbUrl = dbProperties.getProperty("MYSQL_DB_URL");
            String dbUsername = dbProperties.getProperty("MYSQL_DB_USERNAME");
            String dbPassword = dbProperties.getProperty("MYSQL_DB_PASSWORD");

            try {
                Class.forName(dbDriver);
                connection = DriverManager.getConnection(dbUrl, dbUsername, dbPassword);
            } catch (ClassNotFoundException e) {
                throw new SQLException("JDBC Driver not found: " + e.getMessage(), e);
            }

        } catch (IOException e) {
            throw new SQLException("Failed to load DB properties: " + e.getMessage(), e);
        }
    }

    private String executeStoredProcedure(String reportType) throws SQLException {

        if (connection == null) {
            throw new SQLException("Database connection not established.");
        }

        CallableStatement call;
        ResultSet resultSet;
        String sqlCall;

        switch (reportType) {
            case "SumServlet":
                sqlCall = "{CALL Get_The_Sum_Of_All_Parts_Weights()}";
                break;
            case "MaxValueServlet":
                sqlCall = "{CALL Get_The_Maximum_Status_Of_All_Suppliers()}";
                break;
            case "TotalNumberShipments":
                sqlCall = "{CALL Get_The_Total_Number_Of_Shipments()}";
                break;
            case "NameNumJobMostWorkers":
                sqlCall = "{CALL Get_The_Name_Of_The_Job_With_The_Most_Workers()}";
                break;
            case "NameStatusSupplier":
                sqlCall = "{CALL List_The_Name_And_Status_Of_All_Suppliers()}";
                break;
            default:
                throw new SQLException("Invalid report type: " + reportType);
        }

        call = connection.prepareCall(sqlCall);
        resultSet = call.executeQuery();

        String htmlTable = generateHtmlTable(resultSet);

        resultSet.close();
        call.close();

        return htmlTable;
    }

    private String generateHtmlTable(ResultSet rs) throws SQLException {
        StringBuilder html = new StringBuilder();

        html.append("<table border='1' style='border-collapse: collapse; width: fit-content;'>");

        int colCount = rs.getMetaData().getColumnCount();

        // Header
        html.append("<tr>");
        for (int i = 1; i <= colCount; i++) {
            html.append("<th style='background-color:red; padding:8px;'>")
                .append(rs.getMetaData().getColumnName(i))
                .append("</th>");
        }
        html.append("</tr>");

        // Rows
        while (rs.next()) {
            html.append("<tr>");
            for (int i = 1; i <= colCount; i++) {
                html.append("<td style='padding:8px;'>")
                    .append(rs.getString(i))
                    .append("</td>");
            }
            html.append("</tr>");
        }

        html.append("</table>");
        return html.toString();
    }

    private void closeDatabaseConnection() {
        if (connection != null) {
            try { connection.close(); }
            catch (SQLException e) { e.printStackTrace(); }
        }
    }
}

