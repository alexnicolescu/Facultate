<?php
if(session_status() == PHP_SESSION_NONE)
{
    session_start();
}
include 'SignUpModel.php';

$username="";
$email="";
$password="";
$passwordConfirm="";
$errors="";
$ok=0;

if(isset($_POST['signup']))
{
    $username=$_POST['username'];
    $email=$_POST['email'];
    $password=$_POST['password'];
    $pass=$password;
    $passwordConfirm=$_POST['passwordConfirm'];
    $password=md5($password);
    $passwordConfirm=md5($passwordConfirm);


    if(!filter_var($email,FILTER_VALIDATE_EMAIL))
    {
        $errors=$errors."<br>Invalid email address.";
        $ok=1;
    }

    if($password!==$passwordConfirm){
        $errors=$errors."<br>The two passwords must match.";
        $ok=1;
    }
    if(!preg_match("/^[a-zA-Z0-9]*$/",$pass))
    {
        $errors=$errors."<br>The password may contain only numbers and uppercase/lowercase letters .";
        $ok=1;
    }
    if(!preg_match("/^[a-zA-Z0-9]*$/",$username))
    {
        $errors=$errors."<br>The username may contain only numbers and uppercase/lowercase letters .";
        $ok=1;
    }



    $db=new SignUpModel($username,$email,$password,$passwordConfirm);
    $oki=$db->unicitateEmail();
    if($oki==1)
    {
        $errors=$errors."<br>Email already exists.";
        $ok=1;
    }
    $oki=$db->unicitateNume();
    if($oki==1)
    {
        $errors=$errors."<br>Username already exists.";
        $ok=1;
    }

    if($ok==0)
    {
        $user_id=$db->insertUser();
        $_SESSION['id']=$user_id;
        $_SESSION['username']=$username;
        $_SESSION['email']=$email;
        header('Location:Home.php');
        exit();
    }
    else
    {	$_SESSION['errors']=$errors;
        header("location:SignUp.php");
    }
    //echo "<script type='text/javascript'>window.history.back();</script>";
}
