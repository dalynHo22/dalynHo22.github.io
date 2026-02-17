<%@ page import="jakarta.servlet.http.*,jakarta.servlet.*" %>
<%
    
    String resultMsg = (String) session.getAttribute("tableResults");
    if (resultMsg == null) {
        resultMsg = "";
    }
%>
<!DOCTYPE html>
<html>
<head>
    <title>Data Entry Application</title>
</head>
<body bgcolor="black" text="white">

<center>
    <h1 style="color: orange;">Welcome to the Fall 2025 Project 4 Enterprise System</h1>
    <h2 style="color: cyan;">Data Entry Application</h2>
    <hr>

    <p>
        You are connected to the Project 4 Enterprise System database as a 
        <span style="color: red;">data-entry-level</span> user.<br>
        Enter the data values in a form below to add a new record to the corresponding database table.
    </p>

    <!-- =================== SUPPLIERS FORM =================== -->
    <form method="post" action="DataEntryUser">
        <input type="hidden" name="formType" value="supplier">
        <table border="2" cellpadding="4" cellspacing="0" style="border-color: yellow;">
            <tr bgcolor="black">
                <th colspan="4" style="color: yellow;">Suppliers Record Insert</th>
            </tr>
            <tr bgcolor="black">
                <th>snum</th>
                <th>sname</th>
                <th>status</th>
                <th>city</th>
            </tr>
            <tr bgcolor="black">
                <td><input type="text" name="snum" size="10"></td>
                <td><input type="text" name="sname" size="15"></td>
                <td><input type="text" name="status" size="5"></td>
                <td><input type="text" name="scity" size="15"></td>
            </tr>
            <tr bgcolor="black">
                <td colspan="2" align="center">
                    <input type="submit" value="Enter Supplier Record Into Database"
                           style="background-color: #00FF00; color: black; font-weight:bold; width: 260px;">
                </td>
                <td colspan="2" align="center">
                    <input type="reset" value="Clear Data and Results"
                           onclick="window.location='dataEntryHome.jsp';"
                           style="background-color: red; color: yellow; font-weight:bold; width: 200px;">
                </td>
            </tr>
        </table>
    </form>

    <br>

    <!-- =================== PARTS FORM =================== -->
    <form method="post" action="DataEntryUser">
        <input type="hidden" name="formType" value="part">
        <table border="2" cellpadding="4" cellspacing="0" style="border-color: yellow;">
            <tr bgcolor="black">
                <th colspan="5" style="color: yellow;">Parts Record Insert</th>
            </tr>
            <tr bgcolor="black">
                <th>pnum</th>
                <th>pname</th>
                <th>color</th>
                <th>weight</th>
                <th>city</th>
            </tr>
            <tr bgcolor="black">
                <td><input type="text" name="pnum" size="10"></td>
                <td><input type="text" name="pname" size="15"></td>
                <td><input type="text" name="color" size="10"></td>
                <td><input type="text" name="weight" size="5"></td>
                <td><input type="text" name="pcity" size="15"></td>
            </tr>
            <tr bgcolor="black">
                <td colspan="3" align="center">
                    <input type="submit" value="Enter Part Record Into Database"
                           style="background-color: #00FF00; color: black; font-weight:bold; width: 260px;">
                </td>
                <td colspan="2" align="center">
                    <input type="reset" value="Clear Data and Results"
                           onclick="window.location='dataEntryHome.jsp';"
                           style="background-color: red; color: yellow; font-weight:bold; width: 200px;">
                </td>
            </tr>
        </table>
    </form>

    <br>

    <!-- =================== JOBS FORM =================== -->
    <form method="post" action="DataEntryUser">
        <input type="hidden" name="formType" value="job">
        <table border="2" cellpadding="4" cellspacing="0" style="border-color: yellow;">
            <tr bgcolor="black">
                <th colspan="4" style="color: yellow;">Jobs Record Insert</th>
            </tr>
            <tr bgcolor="black">
                <th>jnum</th>
                <th>jname</th>
                <th>numworkers</th>
                <th>city</th>
            </tr>
            <tr bgcolor="black">
                <td><input type="text" name="jnum" size="10"></td>
                <td><input type="text" name="jname" size="15"></td>
                <td><input type="text" name="numworkers" size="5"></td>
                <td><input type="text" name="jcity" size="15"></td>
            </tr>
            <tr bgcolor="black">
                <td colspan="2" align="center">
                    <input type="submit" value="Enter Job Record Into Database"
                           style="background-color: #00FF00; color: black; font-weight:bold; width: 260px;">
                </td>
                <td colspan="2" align="center">
                    <input type="reset" value="Clear Data and Results"
                           onclick="window.location='dataEntryHome.jsp';"
                           style="background-color: red; color: yellow; font-weight:bold; width: 200px;">
                </td>
            </tr>
        </table>
    </form>

    <br>

    <!-- =================== SHIPMENTS FORM =================== -->
    <form method="post" action="DataEntryUser">
        <input type="hidden" name="formType" value="shipment">
        <table border="2" cellpadding="4" cellspacing="0" style="border-color: yellow;">
            <tr bgcolor="black">
                <th colspan="4" style="color: yellow;">Shipments Record Insert</th>
            </tr>
            <tr bgcolor="black">
                <th>snum</th>
                <th>pnum</th>
                <th>jnum</th>
                <th>quantity</th>
            </tr>
            <tr bgcolor="black">
                <td><input type="text" name="ssnum" size="10"></td>
                <td><input type="text" name="spnum" size="10"></td>
                <td><input type="text" name="sjnum" size="10"></td>
                <td><input type="text" name="quantity" size="5"></td>
            </tr>
            <tr bgcolor="black">
                <td colspan="2" align="center">
                    <input type="submit" value="Enter Shipment Record Into Database"
                           style="background-color: #00FF00; color: black; font-weight:bold; width: 260px;">
                </td>
                <td colspan="2" align="center">
                    <input type="reset" value="Clear Data and Results"
                           onclick="window.location='dataEntryHome.jsp';"
                           style="background-color: red; color: yellow; font-weight:bold; width: 200px;">
                </td>
            </tr>
        </table>
    </form>

    <br><br>

    <!-- ========== EXECUTION RESULTS AREA ========== -->
    <table border="0" cellpadding="4" cellspacing="0" width="80%">
        <tr><th style="color:white;" align="left">Execution Results:</th></tr>
        <tr>
            <td bgcolor="blue" style="color:white; padding:8px;">
                <%= resultMsg %>
            </td>
        </tr>
    </table>

</center>
</body>
</html>

