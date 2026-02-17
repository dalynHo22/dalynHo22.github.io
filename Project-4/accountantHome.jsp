<!DOCTYPE html>

<%
    // Retrieve session attributes
    String tableResults = (String) session.getAttribute("tableResults");
%>

<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Accountant Home - TQT</title>
    <style>
        body {
            background: black;
            color: white;
            text-align: center;
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
        }

        h1 {
            color: #FFD700;
            font-size: 28pt;
            margin: 20px 0;
            font-weight: bold;
        }
        
        h2 {
            color: #00FF00;
            font-size: 20pt;
            margin: 15px 0;
            font-weight: normal;
        }

        hr {
            border: 1px solid white;
            margin: 30px 0;
        }

        .user-info {
            font-size: 16pt;
            margin: 20px 0;
        }

        .user-level {
            color: #00FF00;
            font-weight: bold;
        }

        .query-container {
            background-color: #666666;
            padding: 30px;
            margin: 30px auto;
            width: 90%;
            max-width: 900px;
            text-align: left;
            border-radius: 5px;
        }
        
        .radio-option {
            margin: 25px 0;
            font-size: 16pt;
            color: #0066FF;
            display: flex;
            align-items: flex-start;
        }
        
        input[type="radio"] {
            margin-right: 15px;
            margin-top: 3px;
            cursor: pointer;
            transform: scale(1.2);
        }

        .option-text {
            flex: 1;
        }

        .option-description {
            color: black;
            font-weight: normal;
        }

        .button-container {
            text-align: center;
            margin: 30px 0;
        }

        input[type="submit"], input[type="button"] {
            font-size: 16pt;
            font-weight: bold;
            padding: 12px 25px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin: 0 10px;
            transition: background-color 0.3s;
        }
        
        input[type="submit"] {
            color: #00FF00;
            background-color: #665D1E;
        }
        
        input[type="button"] {
            color: #FF4444;
            background-color: #665D1E;
        }

        input[type="submit"]:hover, input[type="button"]:hover {
            background-color: #7a6d28;
        }

        .results-section {
            margin-top: 40px;
        }

        .results-header {
            font-size: 18pt;
            font-weight: bold;
            margin: 20px 0;
        }

        table {
            font-family: Verdana, sans-serif;
            border: 4px solid grey;
            margin: 20px auto;
            border-collapse: collapse;
            width: 90%;
            max-width: 900px;
        }
        
        th {
            padding: 8px;
            border: 3px solid black;
            background: #FF4444;
            color: black;
            font-weight: bold;
        }
        
        td {
            padding: 8px;
            border: 3px solid black;
            color: black;
        }
        
        tr:nth-child(even) td {
            background-color: lightgray;
        }
        
        tr:nth-child(odd) td {
            background-color: white;
        }
    </style>
    <script>
        function clearResults() {
            document.getElementById("tableResults").innerHTML = "";
        }
    </script>
</head>
<body>
    <h1>Welcome to the Fall 2025 Project 4 Enterprise System</h1>
    <h2>A Servlet/JSP-based Multi-tiered Enterprise Application Using a Tomcat Container</h2>
    <hr>
    
    <div class="user-info">
        You are connected to the Project 4 Enterprise System database as an 
        <span class="user-level">accountant-level</span> user.
    </div>
    <div>Please select the operation you would like to perform from the list below.</div>
    <br>
    
    <div class="query-container">
        <form action="AccountantUser" method="post">
            <div class="radio-option">
                <input type="radio" name="query" value="MaxValueServlet" required>
                <div class="option-text">
                    Get The Maximum Status Value Of All Suppliers 
                    <span class="option-description">(Returns a maximum value)</span>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="query" value="SumServlet">
                <div class="option-text">
                    Get The Total Weight Of All Parts 
                    <span class="option-description">(Returns a sum)</span>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="query" value="TotalNumberShipments">
                <div class="option-text">
                    Get The Total Number of Shipments 
                    <span class="option-description">(Returns the current number of shipments in total)</span>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="query" value="NameNumJobMostWorkers">
                <div class="option-text">
                    Get The Name and Number of Workers of the Job with the Most Workers 
                    <span class="option-description">(Returns two values)</span>
                </div>
            </div>
            <div class="radio-option">
                <input type="radio" name="query" value="NameStatusSupplier">
                <div class="option-text">
                    List The Name And Status Of Every Supplier 
                    <span class="option-description">(Returns a list of supplier names with status)</span>
                </div>
            </div>
            
            <div class="button-container">
                <input type="submit" value="Execute Command">
                <input type="button" value="Clear Results" onclick="clearResults()">
            </div>
        </form>
    </div>
    
    <div class="results-section">
        <div>All execution results will appear below this line.</div>
        <hr>
        <div class="results-header">Execution Results:</div>
        
        <div id="tableResults">
            <%= tableResults != null ? tableResults : ""%>
        </div>
    </div>
</body>
</html>
