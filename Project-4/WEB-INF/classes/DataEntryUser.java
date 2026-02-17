/* Name: Dalyn Ho 
Course: CNT 4714 – Fall 2025 – Project Four
Assignment title: A Three-Tier Distributed Web-Based Application
Date: December 1, 2025
Class: Enterprise Computing*/

import java.io.IOException;
import java.io.InputStream;
import java.sql.*;
import java.util.Properties;

import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

@WebServlet("/DataEntryUser")
public class DataEntryUser extends HttpServlet {

    private Connection conn;

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

        String formType = request.getParameter("formType"); // supplier, part, job, shipment
        String message;

        try {
            establishDatabaseConnection();

            if ("supplier".equals(formType)) {
                message = insertSupplier(request);
            } else if ("part".equals(formType)) {
                message = insertPart(request);
            } else if ("job".equals(formType)) {
                message = insertJob(request);
            } else if ("shipment".equals(formType)) {
                message = insertShipment(request);
            } else {
                message = errorMessage("Unknown form type.");
            }

        } catch (SQLException e) {
            message = errorMessage("Error executing data entry: " + e.getMessage());
        } finally {
            closeDatabaseConnection();
        }

        HttpSession session = request.getSession();
        session.setAttribute("tableResults", message);
        response.sendRedirect("dataEntryHome.jsp");
    }

    // ---------- INSERT METHODS ----------

    private String insertSupplier(HttpServletRequest request) throws SQLException {
        String snum   = request.getParameter("snum");
        String sname  = request.getParameter("sname");
        String status = request.getParameter("status");
        String city   = request.getParameter("scity");

        String sql = "INSERT INTO suppliers (snum, sname, status, city) VALUES (?, ?, ?, ?)";

        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, snum);
            ps.setString(2, sname);
            ps.setInt(3, Integer.parseInt(status));
            ps.setString(4, city);

            int rows = ps.executeUpdate();

            if (rows > 0) {
                return successMessage(
                    "New suppliers record: (" + snum + ", " + sname + ", " + status + ", " + city +
                    ") - successfully entered into database."
                );
            } else {
                return errorMessage("No rows inserted for suppliers.");
            }
        }
    }

    private String insertPart(HttpServletRequest request) throws SQLException {
        String pnum   = request.getParameter("pnum");
        String pname  = request.getParameter("pname");
        String color  = request.getParameter("color");
        String weight = request.getParameter("weight");
        String city   = request.getParameter("pcity");

        String sql = "INSERT INTO parts (pnum, pname, color, weight, city) VALUES (?, ?, ?, ?, ?)";

        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, pnum);
            ps.setString(2, pname);
            ps.setString(3, color);
            ps.setInt(4, Integer.parseInt(weight));
            ps.setString(5, city);

            int rows = ps.executeUpdate();

            if (rows > 0) {
                return successMessage(
                    "New parts record: (" + pnum + ", " + pname + ", " + color + ", " + weight + ", " + city +
                    ") - successfully entered into database."
                );
            } else {
                return errorMessage("No rows inserted for parts.");
            }
        }
    }

    private String insertJob(HttpServletRequest request) throws SQLException {
        String jnum       = request.getParameter("jnum");
        String jname      = request.getParameter("jname");
        String numworkers = request.getParameter("numworkers");
        String city       = request.getParameter("jcity");

        String sql = "INSERT INTO jobs (jnum, jname, numworkers, city) VALUES (?, ?, ?, ?)";

        try (PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, jnum);
            ps.setString(2, jname);
            ps.setInt(3, Integer.parseInt(numworkers));
            ps.setString(4, city);

            int rows = ps.executeUpdate();

            if (rows > 0) {
                return successMessage(
                    "New jobs record: (" + jnum + ", " + jname + ", " + numworkers + ", " + city +
                    ") - successfully entered into database."
                );
            } else {
                return errorMessage("No rows inserted for jobs.");
            }
        }
    }

    // *** UPDATED WITH BUSINESS LOGIC ***
    private String insertShipment(HttpServletRequest request) throws SQLException {
        String snum     = request.getParameter("ssnum");
        String pnum     = request.getParameter("spnum");
        String jnum     = request.getParameter("sjnum");
        String quantity = request.getParameter("quantity");

        int qty;
        try {
            qty = Integer.parseInt(quantity);
        } catch (NumberFormatException e) {
            return errorMessage("Quantity must be an integer.");
        }

        String insertSql = "INSERT INTO shipments (snum, pnum, jnum, quantity) VALUES (?, ?, ?, ?)";

        try (PreparedStatement ps = conn.prepareStatement(insertSql)) {
            ps.setString(1, snum);
            ps.setString(2, pnum);
            ps.setString(3, jnum);
            ps.setInt(4, qty);

            int rows = ps.executeUpdate();

            if (rows > 0) {
                String baseMsg =
                    "New shipments record: (" + snum + ", " + pnum + ", " + jnum + ", " + qty +
                    ") - successfully entered into database.";

                // ---- BUSINESS LOGIC: if quantity >= 100, bump supplier status by 5 ----
                if (qty >= 100) {
                    String updateSql = "UPDATE suppliers SET status = status + 5 WHERE snum = ?";
                    try (PreparedStatement ps2 = conn.prepareStatement(updateSql)) {
                        ps2.setString(1, snum);
                        int updated = ps2.executeUpdate();
                        if (updated > 0) {
                            baseMsg += " Business logic triggered.";
                        } else {
                            baseMsg += " Business logic attempted, but no matching supplier was found.";
                        }
                    }
                } else {
                    baseMsg += " Business logic not triggered.";
                }

                return successMessage(baseMsg);
            } else {
                return errorMessage("No rows inserted for shipments.");
            }
        }
    }

    // ---------- DB CONNECTION HELPERS ----------

    private void establishDatabaseConnection() throws SQLException {
        Properties dbProps = new Properties();
        ServletContext ctx = getServletContext();

        try (InputStream in = ctx.getResourceAsStream("/WEB-INF/conf/dataentry.properties")) {
            if (in == null) {
                throw new SQLException("Properties file not found: /WEB-INF/conf/dataentry.properties");
            }

            dbProps.load(in);

            String driver = dbProps.getProperty("MYSQL_DB_DRIVER_CLASS");
            String url    = dbProps.getProperty("MYSQL_DB_URL");
            String user   = dbProps.getProperty("MYSQL_DB_USERNAME");
            String pass   = dbProps.getProperty("MYSQL_DB_PASSWORD");

            try {
                Class.forName(driver);
            } catch (ClassNotFoundException e) {
                throw new SQLException("JDBC Driver not found: " + e.getMessage(), e);
            }

            conn = DriverManager.getConnection(url, user, pass);
        } catch (IOException e) {
            throw new SQLException("Failed to load DB properties: " + e.getMessage(), e);
        }
    }

    private void closeDatabaseConnection() {
        if (conn != null) {
            try { conn.close(); }
            catch (SQLException e) {
                System.err.println("Error closing data entry DB connection: " + e.getMessage());
            }
        }
    }

    // ---------- MESSAGE HELPERS ----------

    private String successMessage(String msg) {
        // blue box is handled by JSP; we just output the text
        return "<font color='white'><b>" + msg + "</b></font>";
        // JSP already places this inside a blue cell
    }

    private String errorMessage(String msg) {
        return "<font color='yellow'><b>ERROR: " + msg + "</b></font>";
    }
}

