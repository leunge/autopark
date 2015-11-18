<?php
//자동차의 아이피와 폰의 아이피를 받아서 저장 후 자동차에게 주차 명령 내림.

$myip=$_GET['myip'];
$car_ip =$_GET['carip'];

$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);

if(!$db)
 echo "db 연결 실패<br>";

if($myip || $car_ip){
	//빈자리 찾기.
	$sql = "select park_no from parkinglot where car='' limit 1";
	$res = mysql_query($sql, $conn);

	$parkinglot = mysql_result($res, 0);
	//db에 올리기
	if($parkinglot==0){
		echo "<script >alert('FULL');</script>";
	}else{
		$sql="insert into car values('$car_ip','$myip', '$parkinglot')";
		mysql_query($sql, $conn);
		
		

		$sql="UPDATE parkinglot SET car='$car_ip', status='parking' WHERE park_no = '$parkinglot'";
		mysql_query($sql, $conn);
	}
	
}else{
 echo "nodata";
}


//자동차 주차하는 명령어.

//노드를 가져왔다고 침 1   0 
if($parkinglot==0){
		echo "<script >document.location.href = 'get_ip.php';</script>";
	}
else{
	//echo $parkinglot;
	 //$output=shell_exec('shell/test.sh'); echo "$output";
	 
	putenv("DISPLAY=:0.0");
	$shel=("xhost +");
	$data = shell_exec($shel);
	 $shel= "/var/www/shell/park.sh ".$parkinglot." > /dev/null 2>/dev/null &";
	 $data = shell_exec($shel);
	// echo "hello";
	// echo $data;
	echo "<script >document.location.href = 'get_ip.php';</script>";
}
?>