<?php
//선택된 자동차 확인 및 넣을지 뺄지 선택
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);


	$carip =$_GET['ip'];
	//echo $ip;
	$myip=$_SERVER['REMOTE_ADDR'];
	$shel= "shell/test_device.sh $carip > /dev/null 2>/dev/null &";
	$data = shell_exec($shel);
	

$sql = "select car_ip from car where car_ip='$carip'";
$res = mysql_query($sql, $conn);
$is_exist = mysql_result($res, 0);


?>

<html>
<head>
<style type="text/css">
body {
			font-family: Arial, Verdana, sans-serif;
			font-size: 90%;
			color: #666;
			margin-top:100px;
			background-color: #f8f8f8;
	}
.myButton {
	-moz-box-shadow:inset 0px 1px 0px 0px #ffffff;
	-webkit-box-shadow:inset 0px 1px 0px 0px #ffffff;
	box-shadow:inset 0px 1px 0px 0px #ffffff;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #ffffff), color-stop(1, #f6f6f6));
	background:-moz-linear-gradient(top, #ffffff 5%, #f6f6f6 100%);
	background:-webkit-linear-gradient(top, #ffffff 5%, #f6f6f6 100%);
	background:-o-linear-gradient(top, #ffffff 5%, #f6f6f6 100%);
	background:-ms-linear-gradient(top, #ffffff 5%, #f6f6f6 100%);
	background:linear-gradient(to bottom, #ffffff 5%, #f6f6f6 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#ffffff', endColorstr='#f6f6f6',GradientType=0);
	background-color:#ffffff;
	-moz-border-radius:6px;
	-webkit-border-radius:6px;
	border-radius:6px;
	border:1px solid #dcdcdc;
	display:inline-block;
	cursor:pointer;
	color:#666666;
	font-family:Arial;
	font-size:25px;
	font-weight:bold;
	padding:20px 40px;
	text-decoration:none;
	text-shadow:0px 1px 0px #ffffff;
}
.myButton:hover {
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #f6f6f6), color-stop(1, #ffffff));
	background:-moz-linear-gradient(top, #f6f6f6 5%, #ffffff 100%);
	background:-webkit-linear-gradient(top, #f6f6f6 5%, #ffffff 100%);
	background:-o-linear-gradient(top, #f6f6f6 5%, #ffffff 100%);
	background:-ms-linear-gradient(top, #f6f6f6 5%, #ffffff 100%);
	background:linear-gradient(to bottom, #f6f6f6 5%, #ffffff 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#f6f6f6', endColorstr='#ffffff',GradientType=0);
	background-color:#f6f6f6;
}
.myButton:active {
	position:relative;
	top:1px;
}
</style>
</head>
<body><center>
<?php
	if($is_exist){
		echo "<a class='myButton' href='status.php'>status</a><br>";
		echo "<a class='myButton' href='out.php?myip=".$myip."&carip=".$carip."'>&nbsp;&nbsp;o&nbsp;u&nbsp;t&nbsp;&nbsp;</a>";
	}else{
		echo "<a class='myButton' href='park.php?myip=".$myip."&carip=".$carip."'>&nbsp;in&nbsp;</a><br>";
	}


?>
</center></body>
</html>