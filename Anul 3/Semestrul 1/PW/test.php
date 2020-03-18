<?php
$servername="localhost";
$user="admin";
$password="admin";
$database="librarieonline";
$conn=new mysqli($servername,$user,$password,$database);

$sql="SELECT * FROM products WHERE id=1 ";
$result=mysqli_query($conn,$sql);
$row=mysqli_fetch_array($result,MYSQLI_ASSOC);
$menu=$row['description'];
?>

<!DOCTYPE html>

<html>
<head>
	<title>Save</title>
	<meta charset="utf-8">
	<script>
        function saveText(){
            var xr=new XMLHttpRequest();
            var url="saveNewText.php";
            var text=document.getElementById("editable").innerHTML;
            var vars="newText="+text;
            xr.open("POST",url,true);
            xr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
            xr.send(vars);
        }
    </script>
</head>
<body>
	<div id="editable" contenteditable="true" onblur="saveText()">
		<?php echo $menu; ?>
	</div>
</body>
</html>