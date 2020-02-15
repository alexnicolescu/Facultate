<?php
require_once 'HomeController.php';
if(isset($_POST['submitbutton']))
{

    $message=$_POST['message'];
    $email=$_POST['email'];
    $headers="From:$email";
    $to="alex.leonardnl@gmail.com";
    $subject="Contact Form Submit";
    if(mail("alex.leonardnl@gmail.com",$subject,$message,$headers))
        echo "<script type='text/javascript'>alert('Works')</script>";
    header("Location:Contact.php");
}
if($_SESSION['username']==""){
    header("Location:Login.php");
    exit();
}
