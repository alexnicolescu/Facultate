<?php
require_once "HomeController.php";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Contact</title>
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
                <a href="Home.php" class="nav-item nav-link">Home</a>
                <a href="Contact.php" class="nav-item nav-link active">Contact</a>
                <a href="ShoppingCart.php" class="nav-item nav-link">Shopping Cart</a>
                <div class="nav-item dropdown">
                    <a href="#" class="nav-link dropdown-toggle" data-toggle="dropdown">Categories</a>
                    <div class="dropdown-menu">
                        <a href="Fiction.php?category=Fiction" class="dropdown-item">Fiction</a>
                        <a href="NonFiction.php?category=NonFiction" class="dropdown-item">Non Fiction</a>
                    </div>
                </div>
            </div>
            <div class="navbar-nav">
                <a href="Home.php?logout=1" class="nav-item nav-link">Logout</a>
            </div>
        </div>
    </nav>
<div class="container"><h1>Contact us!</h1>
    <p>If you have encountered a problem or if you have suggestions, contact us!</p>
    <p>Phone number:+40-745-5514-009</p>
    <p>Email adress:alex.leonardnl@gmail.com</p>
    <form id="contact" action="ContactController.php" method="post">
        <fieldset><input placeholder="Your username" type="text" name="name" required>
        </fieldset>
        <fieldset><input placeholder="Your Email Address" type="email" name="email" required>
        </fieldset>
        <fieldset>
            <textarea placeholder="Type your Message Here..." name="message" required></textarea>
        </fieldset>
            <button name="submitbutton" class="btn btn-primary btn-block btn-lg" type="submit" data-submit="...Sending" type="text" style="width: auto">Submit</button>
        
    </form>
    
</div>

</body>
</html>