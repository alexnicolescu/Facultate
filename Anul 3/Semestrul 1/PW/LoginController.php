<?php
require_once "SignUpController.php";
include 'Model.php';

if(session_status() == PHP_SESSION_NONE)
{
	session_start();
}

/*if(isset($_SESSION['username']) && $_SESSION['username']!="")
{
	header("Location:Home.php");
	exit();
}*/

if(isset($_POST['loginbtn'])){
	$username=$_POST['username'];
	$password=$_POST['password'];
	$_SESSION['username']=$username;
	$dB=new Model("$username","$password");
	$res =$dB->searchUser();
	if($res==0)
	{
		header("Location:Home.php");
		exit();
	}
	else
	{
		$message="Name or password incorrectly.";
		echo "<script type='text/javascript'>alert('$message');window.history.back();</script>";
	}
}


