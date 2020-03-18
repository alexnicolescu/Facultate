<?php
	require_once "FictionController.php";

	if(isset($_GET['name']))
	{


		$productId=$_GET['name'];
		$product=new ProductModel($productId);
		$product->getDetails();
        $productName=$product->getName();
 		$productQuantity=$product->getQuantity();
 		$productGenre=$product->getGenre();
 		$productDescription=$product->getDescription();
        $productPrice=$product->getPrice();
        $productRating=$product->getRating();
        $productAuthor=$product->getAuthor();
        $productFilename=$product->getFilename();
 		$_SESSION['productName']=$productName;
 		$_SESSION['productQuantity']=$productQuantity;
 		$_SESSION['productGenre']=$productGenre;
 		$_SESSION['productDescription']=$productDescription;
        $_SESSION['productPrice']=$productPrice;
        $_SESSION['productRating']=$productRating;
        $_SESSION['productAuthor']=$productAuthor;
        $_SESSION['productFilename']=$productFilename;
        $_SESSION['productId']=$productId;
	}
	else
	{
		echo "<script type='text/javascript'>alert('Nu există această carte.');</script>";
		header("Location:Fiction.php");
	}