<?php
require_once "HomeController.php";

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Home</title>
    <!-- <link rel="stylesheet" href="home.css">-->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js"></script>
<style type="text/css">
    .bs-example{
        margin: 20px;
    }
</style>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
<div class="bs-example">
    <nav class="navbar navbar-expand-md navbar-light bg-light">
        <a href="#" class="navbar-brand">LibrărieOnline</a>
        <button type="button" class="navbar-toggler" data-toggle="collapse" data-target="#navbarCollapse">
            <span class="navbar-toggler-icon"></span>
        </button>

        <div class="collapse navbar-collapse justify-content-between" id="navbarCollapse">
            <div class="navbar-nav">
                <a href="Home.php" class="nav-item nav-link active">Home</a>
                <a href="Contact.php" class="nav-item nav-link">Contact</a>
                <a href="ShoppingCart.php" class="nav-item nav-link">Shopping Cart</a>
                <div class="nav-item dropdown">
                    <a href="#" class="nav-link dropdown-toggle" data-toggle="dropdown">Categories</a>
                    <div class="dropdown-menu" action="HomeController.php" method="POST">
                        <a href="Fiction.php?category=Fiction" name="fictionBtn" class="dropdown-item">Fiction</a>
                        <a href="NonFiction.php?category=NonFiction" name="nonFictionBtn" class="dropdown-item">Non Fiction</a>
                    </div>
                </div>
            </div>
            <div class="navbar-nav">
                <a href="Home.php?logout=1" class="nav-item nav-link">Logout</a>
            </div>
        </div>
    </nav>
    <br>
    <br>
    <div class="text-center">
    <img class="img-rounded" src="images/new-fantasy-books-2017.jpg" alt="An image with a book" style="height=330">
    <p><q><i>Books are the quietest and most constant of friends; they are the most accessible and wisest of counselors, and the most patient of teachers.</i></q>
        <strong>(Charles W. Eliot)</strong>
    </p>
</div>
</div>
</body>
</html>