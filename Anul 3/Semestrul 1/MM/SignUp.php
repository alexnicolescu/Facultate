<?php
require_once 'SignUpController.php';
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Sign Up</title>
    <!--<link rel="stylesheet" href="signup.css"> -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
</head>
<body>
    <div class="container">
        <div class="row">
            <div class="col-md-4 offset-md-4">
                <form action="SignUpController.php" method="POST">
                    <div class="form-group">
                        <h1>Create an account</h1>
                        <p>Please fill in this form to create an account.</p>
                        <hr>
                        <?php
                        if(isset($_SESSION["errors"]))
                        {
                            $error=$_SESSION["errors"];
                            echo "<span>$error</span>";
                            unset($_SESSION['errors']);
                        }
                        ?>
                        <br>
                        <br>
                    </div >
                    <div class="form-group">
                        <label><b>Email</b></label>
                        <input type="text" class="form-control form-control-lg" placeholder="Enter email" id="email" name="email" value="<?php echo $email;?>" required>
                    </div >

                    <div class="form-group">
                        <label><b>Username</b></label>
                        <input type="text"class="form-control form-control-lg"  placeholder="Enter username" id="username" name="username" value="<?php echo $username;?>" required>
                    </div >

                    <div class="form-group">
                        <label><b>Password</b></label>
                        <input type="password" class="form-control form-control-lg" placeholder="Enter password " id="password" name="password" required>
                    </div >

                    <div class="form-group">
                        <label><b>Repeat password</b></label>
                        <input type="password" class="form-control form-control-lg" placeholder="Repeat password" id="passwordConfirm" name="passwordConfirm" required>
                    </div>
                    <div class="form-group">
                        <button type="submit" class="btn btn-primary btn-block btn-lg" name="signup" >Sign up</button>
                   </div class="form-group">

                   <p class="text-center">
                    <a href="Login.php" name="s">Are you a member?Sign in</a>
                </p>
            </form>

        </div>

    </div>
</div>
</body>
</html>