<?php
require_once "FictionController.php";
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Shopping Cart</title>
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
                <a href="Contact.php" class="nav-item nav-link ">Contact</a>
                <a href="ShoppingCart.php" class="nav-item nav-link active">Shopping Cart</a>
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
<div>
    <?php
    if(!isset($_SESSION['tab']))
    {
        echo "<h1 style='text-align: center'>Your shopping cart is empty!</h1>";
    }
    else {
        echo "<table class='table'>
<tr>
                <th scope='row' >Name</th>
                <th scope='row' >Quantity</th>
                <th scope='row' >Price</th>
                <th scope='row' >Total</th>
                <th scope='row' >Action</th></tr>
                <tbody>"
                ;
        $total = 0;
        $array = $_SESSION['tab'];
        $i=0;
        if (is_array($array)) {
            foreach ($array as $item) {
                $name = $item['name'];
                $quantity = $item['quantity'];
                $price = $item['price'];
                $val=number_format($quantity * $price, 2);
                echo "
            <tr>
                <td>$name</td>
                <td>$quantity</td>
                <td>$price</td>
                <td>$val</td>
                <td><a href='ShoppingCart.php?action=delete&name=$name'>Remove</td>
            </tr>";

                $total = $total + ($item['quantity'] * $item['price']);
                $i=$i+1;
            }
            echo "</tbody><tr><tbody>
            <td>Total</td>
            <td>$total</td>
         
</tbody></tr></table>";
        }
    }
    ?>
</div>
<div id="id01" class="modal" tabindex="-1" role="dialog" style="position: fixed;z-index: 1;left:0;top:0;width: 100%;height: 100%;overflow: auto">
    <form class="modal-content" action="ShoppingCartController.php" method="POST" enctype="multipart/form-data">
        <div class="modal-dialog" role="document">
            <div class="modal-content" style="margin: 5% auto 15% auto">
                <div class="modal-header text-center">
        <h4 class="modal-title w-100 font-weight-bold">Checkout</h4>
        </div>
        <div class="modal-body mx-3">
            <div class="md-form mb-5">
                 <i class="fas fa-user prefix grey-text"></i>
            <label for="ccnum">Your credit card number</label>
            <input type="text" placeholder="1111-2222-3333-4444" size="19" name="credit" required >
        </div>
        <div class="md-form mb-5">
                 <i class="fas fa-user prefix grey-text"></i>
            <label for="expmonth">Expiration month</label>
            <input type="number" min="1" max="12" required>
        </div>
        <div class="md-form mb-5">
                 <i class="fas fa-user prefix grey-text"></i>
            <label for="expyear">Expiration year</label>
            <input type="number" min="2020" required></div>
            <div class="md-form mb-5">
                 <i class="fas fa-user prefix grey-text"></i>
            <label for="name">Name written on the card.</label>
            <input type="text" required></div>
            <div class="md-form mb-5">
                 <i class="fas fa-user prefix grey-text"></i>
            <label for="ccv">CCV</label>
            <input type="number" min="100" max="9999" required></div>
            <button class="btn btn-primary btn-block btn-lg" style="width: auto;" type="submit" id="checkbtn" name="checkbtn">Buy</button>
        </div>
    </div>
    </div>
    </form>
</div>
<br>
<?php
if(isset($_SESSION['tab'])) { ?>
    <button onclick="document.getElementById('id01').style.display='block'" style="width:auto;float:right" class="btn btn-primary btn-block btn-lg">Enter your credentials
    </button>
    <?php
}
//eventual sa muti intr-un alt fisier script-ul

?>
<br>
<br>
<script>

    var modal = document.getElementById('id01');


    window.onclick = function(event) {
        if (event.target == modal) {
            modal.style.display = "none";
        }
    }
</script>
<?php
if(isset($_GET['action']))
{
    if($_GET['action']=="delete")
    {
        $j=0;
        foreach ($_SESSION['tab'] as $key)
        {
            if($key['name']==$_GET['name'])
            {
                unset($_SESSION['tab'][$j]);
                $_SESSION['dim']=$_SESSION['dim']-1;
                if($_SESSION['dim']==0)
                {
                    unset($_SESSION['dim']);
                    unset($_SESSION['tab']);
                }
                echo"<script>window.location='ShoppingCart.php'</script>";
                break;
            }
            $j++;
        }
    }
}
?>
</body>
</html>