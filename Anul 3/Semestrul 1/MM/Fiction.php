<?php
require_once 'FictionController.php';
?>
<!DOCTYPE html>
<html>
<head>

    <meta charset="UTF-8">
    <title>Fiction</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
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

    <div class="container" style="margin-right: 0%;margin-bottom: 0%;margin-left: 0%">
        <div class="row" >
            <div class="col-sm">
                <?php
                if($_SESSION['username']=="admin")
                {
                    ?> <button  onclick="document.getElementById('id01').style.display='block'" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg">Add book </button>
                    <form method="POST" class="form-inline" style="margin-bottom: 3px" action="FictionController.php">
                        <input type="search"  name="genreName" placeholder="New Genre"  required>
                        <input type="submit" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg"  name="addGenre" value="Add genre">
                    </form>
                    <form method="POST" class="form-inline" style="margin-bottom: 3px" action="FictionController.php">
                        <select id="genre3" name="genre3">
                            <?php
                            foreach ($_SESSION['genres'] as $elem)
                            {
                                $value=$elem['genre'];

                                echo " <option value='$value'>$value</option>";

                            }
                            ?>
                        </select>
                        <input type="submit" name="delGenre" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg" value="Delete genre">
                    </form>

                    <?php
                }
                ?>

                <form method="POST" style="margin-bottom: 3px" class="form-inline" action="FictionController.php">
                    <input type="search" name="name" placeholder="Book name"  required>
                    <input type="submit" name="searchbtn" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg" value="Search">
                </form>
                <form method="POST" class="form-inline" style="margin-bottom: 3px" action="FictionController.php">
                    <select id="genre2" name="genre2">
                        <option value="All">All</option>
                        <?php
                        foreach ($_SESSION['genres'] as $elem)
                        {
                            $value=$elem['genre'];

                            echo " <option value='$value'>$value</option>";

                        }
                        ?>

                    </select>
                    <input type="submit" name="searchbtn2" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg"  value="Search after genre">
                </form>
            </div>
            <div class="col-sm" >
                <?php
                if(isset($_SESSION["array"]))
                {
                    echo "<ul class='list-group'>";
                    foreach ($_SESSION["array"] as $array)
                    {
                        $id=$array['id'];
                        $name=$array['name'];
                        echo "<li class='list-group-item'><a href='getProduct.php?name=$id' style='text-decoration: none ;color:black;visited:black'>$name</a></li>";
                    }
                    echo "</ul>";
                    echo "</div>";
                    unset($_SESSION["array"]);
                }
                ?>
            </div>
        </div>
    </div>
    <div id="id01" class="modal">
        <form class="modal-content"  tabindex="-1" role="dialog" action="FictionController.php" method="POST" enctype="multipart/form-data">
           <div class="modal-dialog" role="document">
            <div class="modal-content" style="margin: 5% auto 15% auto">
                <div class="modal-header text-center">
                    <h4 class="modal-title w-100 font-weight-bold">Add book</h4>
                </div>

                <div class="modal-body mx-3">
                    <?php
                    if(isset($_SESSION["errors"]))
                    {
                        echo "<script type='text/javascript'>alert('No image added.')</script>";
                        unset($_SESSION['errors']);

                    }elseif (isset($_SESSION['unique'])) {
                        echo "<script type='text/javascript'>alert('Book already exists')</script>";
                        unset($_SESSION['unique']);
                    }
                    elseif(isset($_SESSION['productSaved']))
                    {
                        if($_SESSION['productSaved']===TRUE){
                            echo "<script type='text/javascript'>alert('The details have been saved.')</script>";
                            $_SESSION['productSaved']=FALSE;
                        }
                    }
                    ?>
               
                
                    <label for="name"><b>Name:</b></label>
                    <fieldset><input type="text" id="name" placeholder="Enter the name of the book" name="name" required></fieldset>
                    <label for="author"><b>Author</b></label>
                    <fieldset><input type="text" id="author" placeholder="Enter the name of the author" name="author" required></fieldset>

                    <label for="quantity"><b>Quantity:</b></label>
                    <fieldset><input type="number" id="quantity" name="quantity" min="0" required></fieldset>
                    <label for="price"><b>Price:</b></label>
                    <fieldset><input type="number" step=any id="price" name="price" min="0" required></fieldset>
                    <label for="genre"><b>Genre</b></label>
                    <fieldset> <form class="form-inline">
                        <select id="genre" name="genre">
                            <?php
                            foreach ($_SESSION['genres'] as $elem)
                            {
                                $value=$elem['genre'];

                                echo " <option value='$value'>$value</option>";

                            }
                            ?>
                        </select>
                    </form></fieldset>
                    <fieldset>
                        <textarea placeholder="Type your Message Here..." name="description" required></textarea>
                    </fieldset>
                    <label for="file">Image</label>
                    <fieldset><input type="file"   id="file" name="file[]"></fieldset>


                    <button  type="submit" id="submit" style="width: auto;padding: 5px 5px;font-size:16px"class="btn btn-primary btn-block btn-lg" name="addbtn">Add</button>
                </div>

            </form>


        </div>
        <script>
            var modal = document.getElementById('id01');

            window.onclick = function(event) {
                if (event.target == modal) {
                    modal.style.display = "none";
                }
            }
        </script>

    </body>
    </html>
