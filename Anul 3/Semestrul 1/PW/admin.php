<?php
require_once "HomeController.php";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Admin Options For Categories</title>
    <link rel="stylesheet" href="home.css">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
<div class="topnav">
    <a href="Home.php">Home</a>
    <div class="dropdown">
        <button class="dropbtn" href="#">Categories</button>
        <div class="dropdown-content">
            <form  method="POST" action="HomeController.php">
                <?php
                foreach ($_SESSION['categories'] as $elem)
                {
                    $cat=$elem['category'];
                    ?>
                    <input  type='submit' name=<?php echo "$cat" ?> value=<?php echo "$cat" ?>><?php
                }
                ?>
            </form>
        </div>
    </div>
    <a   href="Contact.php">Contact</a>
    <a href="Home.php?logout=1" style="float:right">Log out</a>
    <a href="ShoppingCart.php"  style="float:right">Shopping Cart</a>
    <?php
    if($_SESSION['username']=="admin")
    {
        echo "<a href=\"admin.php\" class='active'  style=\"float:left\">Admin Options For Categories</a>";
    }
    ?>
</div>
<form method="POST" action="HomeController.php">
    <input type="search" name="categoryName" placeholder="New Category"  required>
    <input type="submit" style="width: auto" name="addCategory" value="Add category">
</form>
<form method="POST" action="FictionController.php">
    <select id="category" name="category">
        <?php
        foreach ($_SESSION['categories'] as $elem)
        {
            $value=$elem['category'];

            echo " <option value='$value'>$value</option>";

        }
        ?>
    </select>
    <input type="submit" style="width: auto" name="delCategory" value="Delete category">
</form>
</body>
</html>