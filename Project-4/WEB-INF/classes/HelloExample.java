import java.io.*;
import jakarta.servlet.*;
import jakarta.servlet.http.*;

public class HelloExample extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws IOException, ServletException {

        response.setContentType("text/html");
        PrintWriter out = response.getWriter();
        out.println("<h1>Hello from HelloExample Servlet!</h1>");
    }
}

