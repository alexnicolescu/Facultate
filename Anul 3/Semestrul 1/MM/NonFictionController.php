<?php
include 'config.php';
include 'FictionModel.php';
include 'getProductModel.php';
require_once 'HomeController.php';

$productSaved = FALSE;
$errors="";
$ok=0;
$productName="";
$productQuantity="";
$productGenre="";
$productAuthor="";
$productDescription="";
$data=new GenreModel("smt");
$_SESSION['genres']=$data->getGenres("NonFiction");
$productCategory="NonFiction";
if(isset($_POST['addbtn']))
{
    $productName=$_POST['name'];
    $productQuantity=$_POST['quantity'];
    $productGenre=$_POST['genre'];
    $productPrice=$_POST['price'];
    $productAuthor=$_POST['author'];
    $productDescription=$_POST['description'];
    if(!is_dir(UPLOAD_DIR))
        mkdir(UPLOAD_DIR,0777,true);
    $filenamesToSave=[];
    $allowedMimeTypes=explode(',', UPLOAD_ALLOWED_MIME_TYPES);
    if (!empty($_FILES)) {
        if (isset($_FILES['file']['error'])) {
            foreach ($_FILES['file']['error'] as $uploadedFileKey => $uploadedFileError) {
                if ($uploadedFileError === UPLOAD_ERR_NO_FILE) {
                    $ok=1;
                } elseif ($uploadedFileError === UPLOAD_ERR_OK) {
                    $uploadedFileName = basename($_FILES['file']['name'][$uploadedFileKey]);

                    if ($_FILES['file']['size'][$uploadedFileKey] <= UPLOAD_MAX_FILE_SIZE) {
                        $uploadedFileType = $_FILES['file']['type'][$uploadedFileKey];
                        $uploadedFileTempName = $_FILES['file']['tmp_name'][$uploadedFileKey];

                        $uploadedFilePath = rtrim(UPLOAD_DIR, '/') . '/' . $uploadedFileName;

                        if (in_array($uploadedFileType, $allowedMimeTypes)) {
                            if (!move_uploaded_file($uploadedFileTempName, $uploadedFilePath)) {

                                $ok=1;
                            } else {
                                $filenamesToSave[] = $uploadedFilePath;
                            }
                        } else {

                            $ok=1;
                        }
                    } else {

                        $ok=1;
                    }
                }
            } 
        }
    }
    if($ok==0){
        $db=new FictionModel($productName,$productQuantity,$productGenre,$productPrice,$productAuthor,$filenamesToSave[0],$productDescription,$productCategory);
        if($db->unicitateNume()==1)
        {
            $_SESSION['unique']=1;
            header("Location:NonFiction.php?category=$productCategory");
            exit(); 
        }
        $db->insertProduct();
        //$db->insertImages($filenamesToSave);
        $productSaved=TRUE;
        $_SESSION['productSaved']=$productSaved;
        $productName=$productQuantity=$productGenre=NULL;
    }
    else
    {
        $_SESSION['errors']=$errors;
    }
    header("Location:NonFiction.php?category=$productCategory");
}
if(isset($_POST["searchbtn"]))
 {
    $productName=$_POST["name"];
    $db=new FictionIdModel($productName);
    $db->getDetails();
    $id=$db->getId();
    $ok=$db->search();
    if($ok>0)
    {   
        header("Location:getProduct.php?name=$id");
    }
    else
    {
        echo "<script type='text/javascript'>alert('The book doesn\'t exist.');window.history.back();</script>";

    }
 }
 if(isset($_POST["searchbtn2"]))
 {
    $productGenre=$_POST["genre2"];
    $db=new FictionModelForGenreSearch($productGenre);
    $array=$db->search2("NonFiction");
    $_SESSION['array']=$array;
    header("Location:NonFiction.php?category=$productCategory");
 }
if(isset($_POST['buybtn']))
{
    $desiredQuantity=$_POST['desiredQuantity'];
    if(!isset($_SESSION['dim']))
        $_SESSION['dim']=0;
    $tab=array();
    if($desiredQuantity<$_SESSION['productQuantity'])
    {
        $tab['name']=$_SESSION['productName'];
        $tab['quantity']=$desiredQuantity;
        $tab['price']=$_SESSION['productPrice'];
        $_SESSION['tab'][$_SESSION['dim']]=$tab;
        $_SESSION['dim']=$_SESSION['dim']+1;
        echo "<script type='text/javascript'>window.history.back();</script>";
    }
    else
        echo "<script type='text/javascript'>alert('Not enough books.');window.history.back();</script>";
}
if(isset($_POST['updatebtn']))
{
    $updateQuantity=$_POST['updateQuantity'];
    $db=new FictionModelSon($_SESSION['productName']);
    $db->update($updateQuantity);
    echo "<script type='text/javascript'>window.history.back();</script>";
}
if(isset($_POST['deletebtn']))
{
    $db=new FictionModelSon($_SESSION['productName']);
    $db->delete();
    $db2=new ModelRating($_SESSION['username']);
    $db2->deleteRow($_SESSION['productName']);
    header("Location:NonFiction.php?category=$productCategory");
}
if(isset($_POST['ratebtn']))
{
    $val=$_POST['rating'];
    $db=new ModelRating($_SESSION['username']);
    $vari=$db->getRating($_SESSION['productName']);
    if($vari==NULL || $vari!=$val)
    {
        $db2=new FictionModelForRating($_SESSION['productName']);
        $db->updateRating($_SESSION['productName'],$val);
        $rate=$db->getRates($_SESSION['productName']);
        $rate=$rate/$db->getNr();
        $db2->updateRating($rate);
    }
    echo "<script type='text/javascript'>window.history.back();</script>";

}
if(isset($_POST['addGenre']))
{
    $text=trim($_POST['genreName']);
    $db=new GenreModel($text);
    $db->insertGenre("NonFiction");
    header("Location:NonFiction.php?category=$productCategory");
}
if(isset($_POST['delGenre']))
{
    $genre=$_POST['genre3'];
    $db=new GenreModel($genre);
    $db->deleteGenre();
    $db2=new FictionGenreModel($genre);
    $db2->deleteGenre();
    header("Location:NonFiction.php?category=$productCategory");
}
if($_SESSION['username']==""){
    header("Location:Login.php");
}