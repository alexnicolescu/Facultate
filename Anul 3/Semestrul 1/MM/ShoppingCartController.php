<?php
require_once 'FictionController.php';
include 'ShoppingCartModel.php';
if(isset($_POST['checkbtn']))
{
    foreach ($_SESSION['tab'] as $elem) {
        $db =new ShoppingCartModel($elem['name']);
        $db->updateStock($elem['quantity']);
    }
    unset($_SESSION['tab']);
    unset($_SESSION['dim']);
    header("Location:ShoppingCart.php");
    exit();
}
if($_SESSION['username']==""){
    header("Location:Login.php");
    exit();
}
/*
$verificare=0;
if(isset($_SESSION['tab'])){
	$array=$_SESSION['tab'];
	$nr=$_SESSION['dim'];
	for ( $j = 0 ; $j< $nr ; $j++ ) {
		if(isset($_POST['delbtn$j']))
		{
			$_SESSION['dim']=$_SESSION['dim']-1;
			$verificare=1;
            break;
		}

	}
	if($nr==1 && $verificare==1)
	{
		unset($_SESSION['tab']);
		unset($_SESSION['dim']);
	}
	else
	$_SESSION['tab']=$array;

	header("Location:ShoppingCart.php");
	$_SESSION['verificare']=$verificare;
	exit();
}*/