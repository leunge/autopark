
<?php
//$shel= "curl 192.168.1.1/con.txt | awk '{print $3, $4 ". "\"<br>\"". "}' |grep '[*]'";
$shel= "curl 192.168.1.1/con.txt |grep '*' | awk '{print $3 ". "\", \"". " $4". "\"<br>,\"". "}' ";
$data = shell_exec($shel);


$strTok =explode(',' , $data);

?>


<html>
<head>
<link rel="stylesheet" type="text/css" href="css/table-general.css" />
    <style type="text/css">
	body {
			font-family: Arial, Verdana, sans-serif;
			font-size: 90%;
			color: #666;
			margin-top:100px;
			background-color: #f8f8f8;
	}
.myButton {
	-moz-box-shadow: 0px 10px 14px -7px #276873;
	-webkit-box-shadow: 0px 10px 14px -7px #276873;
	box-shadow: 0px 10px 14px -7px #276873;
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #599bb3), color-stop(1, #408c99));
	background:-moz-linear-gradient(top, #599bb3 5%, #408c99 100%);
	background:-webkit-linear-gradient(top, #599bb3 5%, #408c99 100%);
	background:-o-linear-gradient(top, #599bb3 5%, #408c99 100%);
	background:-ms-linear-gradient(top, #599bb3 5%, #408c99 100%);
	background:linear-gradient(to bottom, #599bb3 5%, #408c99 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#599bb3', endColorstr='#408c99',GradientType=0);
	background-color:#599bb3;
	-moz-border-radius:8px;
	-webkit-border-radius:8px;
	border-radius:8px;
	display:inline-block;
	cursor:pointer;
	color:#ffffff;
	font-family:Arial;
	font-size:12px;
	font-weight:bold;
	padding:6px 9px;
	text-decoration:none;
	text-shadow:0px 1px 0px #3d768a;
}
.myButton:hover {
	background:-webkit-gradient(linear, left top, left bottom, color-stop(0.05, #408c99), color-stop(1, #599bb3));
	background:-moz-linear-gradient(top, #408c99 5%, #599bb3 100%);
	background:-webkit-linear-gradient(top, #408c99 5%, #599bb3 100%);
	background:-o-linear-gradient(top, #408c99 5%, #599bb3 100%);
	background:-ms-linear-gradient(top, #408c99 5%, #599bb3 100%);
	background:linear-gradient(to bottom, #408c99 5%, #599bb3 100%);
	filter:progid:DXImageTransform.Microsoft.gradient(startColorstr='#408c99', endColorstr='#599bb3',GradientType=0);
	background-color:#408c99;
}
.myButton:active {
	position:relative;
	top:1px;
}


</style>

</head>

<body>

<center>
<h1>Choose your car</h1><br>
<table class="general">
			<tr class="head">
				<th>Select</th>
				<th>car_ip</th>
				<th>Name</th>
			</tr>
<?php
	//FOR문 돌려서 아이피와 이름을 보여줌.
	for($i = 0 ; $i < count($strTok) ; $i ++){
			echo "<tr><td><a href='select.php?ip=".$strTok[$i]."' class='myButton'>My Car</a></td>";
			echo " <td>". $strTok[$i++];
			if($i == count($strTok)){
				echo "</td><td>";
			}else{
				echo "</td><td>". $strTok[$i];
			}
			echo "</td></tr>";
	}
	//선택시 깜빡이게 함.
	//페이지 변경. IP 전송.
?>

</table>
<br><a href='admin.php' >admin page</a></body>
</center>

</html>