<?php
require_once 'getProductController.php';
$newText=trim($_POST['newText']);
$newText2=$_POST['newText2'];
$newText3=trim($_POST['newText3']);
$newText4=trim($_POST['newText4']);
$newText5=trim($_POST['newText5']);
$name=$_SESSION['productId'];
if($newText !="")
{	
	
	$db=new ProductModel($name);
	$db->updateDB($newText);
}
if($newText2 !="") {
    $db = new ProductModel($name);
    $db->updateDBprice($newText2);

}
if($newText3 !=""){
    $db = new ProductModel($name);
    $db->updateDBname($newText3);


}
if($newText4 !=""){
    $db = new ProductModel($name);
    $db->updateDBauthor($newText4);

}
if($newText5 !="")
{
    $db = new ProductModel($name);
    $db->updateDBgenre($newText5);

}