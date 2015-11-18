<?php
//선택된 자동차 확인 및 넣을지 뺄지 선택
	$ip =$_GET['ip'];
	//echo $ip;
	
	$shel= "shell/test_device.sh $ip > /dev/null 2>/dev/null &";
	$data = shell_exec($shel);
?>

<html>
<head>

</head>
<body>
choose your car<br>
<?php
echo "<a href='test3.php?ip=".$ip."&type=in'><button type='button'>in</button></a>";

echo "<a href='test3.php?ip=".$ip."&type=out'><button type='button'>out</button></a>";
?>
</body>
</html>