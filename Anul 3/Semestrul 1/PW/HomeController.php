<?php
require_once 'LoginController.php';
include 'HomeModel.php';
$db=new HomeModel("name");
$_SESSION['categories']=$db->getCategories();
if($_SESSION['username']==""){
	header("Location:Login.php");
	exit();
}
if(isset($_GET['logout']))
{
	session_destroy();
	unset($_SESSION['username']);
	header("Location:Login.php");
	exit();
}
if(isset($_POST['addCategory']))
{
    $category=$_POST['categoryName'];
    $db=new HomeModel($category);
    $db->addCategory();
    header("Location:admin.php");
}
if(isset($_POST['delCategory']))
{
    $category=$_POST['category'];
    $db2=new FictionCategoryModel($category);
    $db2->deleteCategory();
    $db2->deleteCategoryFromGenres();
    $db=new HomeModel($category);
    $db->deleteCategory();
    header("Location:admin.php");
}

if(isset($_SESSION['fictionBtn']))
{
    $_SESSION['category']="Fiction";
    header("Location:Fiction.php?category=Fiction");
}
if(isset($_SESSION['nonFictionBtn']))
{
    $_SESSION['category']="NonFiction";
    header("Location:Fiction.php?category=NonFiction");
}

/*
foreach($_SESSION['categories'] as $elem)
{
    $name=$elem['category'];
    if(isset($_POST[$name]))
    {
        $_SESSION['category']=$name;
        header("Location:Fiction.php?category=$name");
    }
}*/
