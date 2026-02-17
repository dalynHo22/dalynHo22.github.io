// Hide forms on page load
//A method: a function that you CALL like .alert()
//A property: a variable attached to an object. like .onload for the window global object 
//element.style.display = "none"; This accesses the inline CSS styles of that element. Like editing the element’s style="" attribute directly.
// in css, display: none;
//The inline style overrides the CSS file. Inline > CSS file. .style.display only changes inline styles
window.onload = function () {
    document.getElementById("newContent").style.display = "none";
    document.getElementById("filterContent").style.display = "none";
}

/* =====TOGGLE FILTER MENU===== */
function showFilter() {
    let filterForm = document.getElementById("filterContent");

    //checks filterContent: if hidden -> show it, if visible -> hide it. 
    if (filterForm.style.display === "none") {
        filterForm.style.display = "block";
    } else {
        filterForm.style.display = "none";
    }
}

/* =====TOGGLE ADD ARTICLE FORM===== */
function showAddNew() {
    let newForm = document.getElementById("newContent");

    //if newContent button is clicked then form appears, click again then form disappears
    if (newForm.style.display === "none") {
        newForm.style.display = "block";
    } else {
        newForm.style.display = "none";
    }
}

/* =====FILTER ARTICLES===== */
function filterArticles() {
    //.checked returns true or false
    let showOpinion = document.getElementById("opinionCheckbox").checked;
    let showRecipe = document.getElementById("recipeCheckbox").checked;
    let showUpdate = document.getElementById("updateCheckbox").checked;

    //this selects all <article> elements inside #articleList. There is now a list of every article
    //querySelectorAll() method returns a static NodeList containing all elements that match a specified CSS selector
    //NodeList: an array-like object that represents a collection of DOM nodes (elements, text, or comments).
    //NodeLists are array-like, not an array so doesn't support all array methods like map(), filter(), or reduce() but does support forEach()
    let articles = document.querySelectorAll("#articleList article");

    //for each article in the list 
    /*Checks if article has class "opinion"
      If yes:
        If checkbox is checked -> show it
        If checkbox unchecked -> hide it */
    //classList is a property in JavaScript is a read-only property that provides a live collection (DOMTokenList) of an HTML element's CSS classes
    //contains() is a method of the DOMTokenList interface returns a boolean value — true if the underlying list contains the given token, otherwise false
    articles.forEach(article => {
        if (article.classList.contains("opinion")) {
                                    //ternary operator
                                    /*If showOpinion is true → use "block"
                                      Else → use "none" */
            article.style.display = showOpinion ? "block" : "none";
        }
        else if (article.classList.contains("recipe")) {
            article.style.display = showRecipe ? "block" : "none";
        }
        else if (article.classList.contains("update")) {
            article.style.display = showUpdate ? "block" : "none";
        }
    });

    /*Every HTML element has a classList property.It represents the element’s classes as a list object
    ex. if you have, html: <div class="box red large"></div>
        then, js: element.classList
        is basically, css: ["box", "red", "large"]
    */

    /*.contains property returns a Boolean; True or False. 
      ex. element.classList.contains("red") asks “Is the class 'red' inside this element’s class list?”*/
}

/* =====ADD NEW ARTICLE===== */
function addNewArticle() {
    //.value gets what the user typed.
    let title = document.getElementById("inputHeader").value;
    let content = document.getElementById("inputArticle").value;

    // Determine which radio button was selected
    let type = "";
    let markerText = "";

    /*Then assign:

      type → used as CSS class

      markerText → displayed label */

    if (document.getElementById("opinionRadio").checked) {
        type = "opinion";
        markerText = "Opinion";
    } 
    else if (document.getElementById("recipeRadio").checked) {
        type = "recipe";
        markerText = "Recipe";
    } 
    else if (document.getElementById("lifeRadio").checked) {
        type = "update";
        markerText = "Update";
    }

    // Basic validation
    if (title === "" || content === "" || type === "") {
        alert("Please complete all fields.");
        return;
    }

    /*The document.createElement() method in JavaScript is used to dynamically create a new HTML element node of a specified type. 
    The newly created element is not automatically added to the document's DOM tree; 
    it must be manually appended to an existing element using methods like appendChild() or append() */

    // Create the new article (DOM Creation)
    //this creates <article class="opinion"> (or recipe/update)
    let newArticle = document.createElement("article");
    newArticle.classList.add(type);
    /*The JavaScript **classList.add()** method is used to add one or more CSS classes to an HTML element's class attribute.
     It is the recommended way to manipulate individual classes as it safely appends new classes without affecting existing ones and automatically handles duplicates.  */

    // Create marker
    //this creates <span class="marker">Opinion</span> (or recipe/update)
    let marker = document.createElement("span");
    marker.classList.add("marker");
    marker.textContent = markerText;

    // Create title
    //creates <h2>User Title</h2>
    let h2 = document.createElement("h2");
    h2.textContent = title;

    // Create paragraph
    //creates <p>User text...</p>
    let p = document.createElement("p");
    p.textContent = content;

    // Append elements
    //The appendChild() method of the Node interface adds a node to the end of the list of children of a specified parent node.
    //This builds the full structure inside <article>
    newArticle.appendChild(marker);
    newArticle.appendChild(h2);
    newArticle.appendChild(p);

    // Add to article list
    //This inserts the new article at the bottom of the list.
    document.getElementById("articleList").appendChild(newArticle);

    // Clear form
    document.getElementById("inputHeader").value = "";
    document.getElementById("inputArticle").value = "";
    document.getElementById("opinionRadio").checked = false;
    document.getElementById("recipeRadio").checked = false;
    document.getElementById("lifeRadio").checked = false;

    // Apply filter again in case some categories are hidden
    /*Example: If user has Recipe unchecked and adds a Recipe we don’t want it to appear.So we run filtering again. */
    filterArticles();
}
