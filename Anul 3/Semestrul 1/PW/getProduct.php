<?php
require_once "getProductController.php";

?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Book</title>
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
    <script>
        function saveText(){
            var xr=new XMLHttpRequest();
            var url="saveNewText.php";
            var text=document.getElementById("editable").innerHTML;
            var vars="newText="+text;
            xr.open("POST",url,true);
            xr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr.send(vars);
            var xr2=new XMLHttpRequest();
            var text2=document.getElementById("editable2").innerHTML;
            var vars2="newText2="+text2;
            xr2.open("POST",url,true);
            xr2.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr2.send(vars2);
            var xr4=new XMLHttpRequest();
            var text3=document.getElementById("editable3").innerHTML;
            var vars3="newText3="+text3;
            xr4.open("POST",url,true);
            xr4.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr4.send(vars3);
            var xr3=new XMLHttpRequest();
            var text4=document.getElementById("editable4").innerHTML;
            var vars4="newText4="+text4;
            xr3.open("POST",url,true);
            xr3.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr3.send(vars4);
            var xr5=new XMLHttpRequest();
            var text5=document.getElementById("editable5").innerHTML;
            var vars5="newText5="+text5;
            xr5.open("POST",url,true);
            xr5.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr5.send(vars5);

        }
    </script>
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


        <div class="text-left">
            <table>
                <tr>
                    <td>Name:</td>
                    <td id="editable3" <?php
                    if($_SESSION['username']=='admin')
                        echo "contenteditable='true'";?>
                    onblur='saveText()'>
                    <?php echo $_SESSION["productName"];?>
                </td>
            </tr>
            <tr>
                <td>Author:</td>
                <td id="editable4" <?php
                if($_SESSION['username']=='admin')
                    echo "contenteditable='true'";?>
                onblur='saveText()'><?php echo $_SESSION["productAuthor"];?></td>
            </tr>
            <tr>
                <td>Quantity:</td>
                <td ><?php echo $_SESSION["productQuantity"];?></td>
            </tr>
            <tr>
                <td>Price:</td>
                <td id="editable2" <?php
                if($_SESSION['username']=='admin')
                  echo "contenteditable='true'";?>
              onblur='saveText()'><?php echo $_SESSION["productPrice"];?></td>
          </tr>
          <tr>
            <td>Genre:</td>
            <td id="editable5" <?php
            if($_SESSION['username']=='admin')
                echo "contenteditable='true'";?>
            onblur='saveText()'><?php echo $_SESSION["productGenre"];?></td>
        </tr>
        <tr>
            <td>Rating:</td>
            <td><?php echo $_SESSION["productRating"];?></td>
        </tr>
        <tr>
            <td>Picture:</td>
            <td>
                <img src="<?php echo $_SESSION['productFilename']; ?>" alt="" style=" max-width: 200px;"/>
            </td>
        </tr>
        <tr>
            <td>Description:</td>
            <td id="editable" <?php
            if($_SESSION['username']=="admin")
                echo "contenteditable='true'  ";
            ?> onblur='saveText()'>
            <?php if(isset($_SESSION['productDescription']))
            echo $_SESSION['productDescription'];?>
        </td>

    </tr>

</table>
<form style="float:right;" class="form-inline" method="POST" action="FictionController.php">
    <input type="number" min="1"  class="form-control" name="desiredQuantity" placeholder="Desired quantity" required>
    <input type="submit" name="buybtn" class="btn btn-primary btn-block btn-lg" style="width: auto;padding: 5px 5px;font-size:16px;float:right" value="Add to shopping cart">
</form>
<form method="POST" class="form-inline"  style="float: left" action="FictionController.php">
    <select  class="form-control" id="rating" name="rating">
        <option value="1">1</option>
        <option value="2">2</option>
        <option value="3">3</option>
        <option value="4">4</option>
        <option value="5">5</option>
    </select>
    <input type="submit" class="btn btn-primary btn-block btn-lg" name="ratebtn" style="width: auto;padding: 5px 5px;font-size:16px;float:right" value="Rate">
</form>
<br>
<br>
<Br>
<?php

if($_SESSION['username']=="admin") {
    ?>
    <form style="float:right" class="form-inline" method="POST" action="FictionController.php">
        <input type="number" min="1"  class="form-control mr-sm-2"  name="updateQuantity" placeholder="Add books to the stock" required>
        <input type="submit" class="btn btn-primary btn-block btn-lg" style="width: auto;padding: 5px 3px 5px 3px;font-size:16px;float:right" name="updatebtn" value="Add">
    </form>

    <br>
    <br>
    <br>

    <form style="float:right" method="POST" action="FictionController.php">
        <input type="submit"  class="btn btn-primary btn-block btn-lg" style="width: auto;padding: 5px 5px;font-size:16px" name="deletebtn" value="Delete">
    </form>
<?php } ?>
<div style=" padding-bottom:50px"></div>
</div>



</body>
</html>