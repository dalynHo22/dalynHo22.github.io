<!DOCTYPE html>
<% 
    String tableResults = (String) session.getAttribute("tableResults");
    String sqlStatement = (String) session.getAttribute("sql");
%>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Root Home - TQT</title>
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
            color: #FFD700;
            font-weight: bold;
        }

        textarea {
            background: #0066FF;
            color: white;
            font-family: Verdana, sans-serif;
            font-size: 15pt;
            width: 90%;
            max-width: 900px;
            height: 300px;
            border: 2px solid #004499;
            border-radius: 5px;
            padding: 10px;
            box-sizing: border-box;
            resize: vertical;
        }

        textarea:focus {
            outline: none;
            border-color: #0088FF;
        }

        .button-container {
            margin: 20px 0;
        }

        input[type="submit"] {
            background: #665D1E;
            color: #00FF00;
            font-weight: bold;
            font-size: 16pt;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin: 0 10px;
            transition: background-color 0.3s;
        }

        input[type="button"] {
            background: #665D1E;
            font-weight: bold;
            font-size: 16pt;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin: 0 10px;
            transition: background-color 0.3s;
        }

        input[type="button"]:nth-of-type(1) {
            color: #FF4444;
        }

        input[type="button"]:nth-of-type(2) {
            color: #FFD700;
        }

        input[type="submit"]:hover, input[type="button"]:hover {
            background: #7a6d28;
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
        function resetForm() {
            document.getElementById("sql").value = "";
        }
        
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
        You are connected to the Project 4 Enterprise System database as a 
        <span class="user-level">root-level</span> user.
    </div>
    <div>Please enter any SQL query or update command in the box below.</div>
    <br>
    
    <form action="RootUser" method="post">
        <textarea id="sql" name="sql" placeholder="Enter your SQL command here..."><%= sqlStatement != null ? sqlStatement : "" %></textarea>
        <br>
        <div class="button-container">
            <input type="submit" value="Execute Command">
            <input type="button" value="Reset Form" onclick="resetForm()">
            <input type="button" value="Clear Results" onclick="clearResults()">
        </div>
    </form>
    
    <div class="results-section">
        <div>All execution results will appear below this line.</div>
        <hr>
        <div class="results-header">Execution Results:</div>
        
        <div id="tableResults">
            <%= tableResults != null ? tableResults : "" %>
        </div>
    </div>
</body>
</html>
