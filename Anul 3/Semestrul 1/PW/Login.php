<?php
require_once 'LoginController.php';
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Login</title>
    <!--<link rel="stylesheet" href="login.css"> -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
</head>
<body>
    
    <div class="container">
      <div class="row">
        <div class="col-md-4 offset-md-4">
            <form action="LoginController.php" method="POST">
                <div class="form-group">
                    <h1>Log in</h1>
                    <hr>
                    <br>
                    <br>
                </div >
                <div class="form-group">
                    <label><b>Username</b></label>
                    <input type="text" id="username" name="username" placeholder="Enter your username" required>
                </div>

                <div class="form-group">
                    <label><b>Password</b></label>
                    <input type="password" id="password" name="password"placeholder="Enter your password" required>
                </div>

                <div class="form-group">
                    <button type="submit" class="btn btn-primary btn-block btn-lg"  name="loginbtn">Log in</button>
                </div>
                
                <p class="text-center"><a href="SignUp.php">If you don't have an account, click here.</a></p>
                

            </form>
        </div>
    </div>
</div>
</body>
</html>