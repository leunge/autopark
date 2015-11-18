<?php
//자동차의 아이피와 폰의 아이피를 받아서 저장 후 자동차에게 주차 명령 내림.

$carip=$_GET['carip'];
echo $carip;
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);

if($db)
 echo "db 연결성공<br>";
else
 echo "db 연결 실패<br>";


//db에서 제거.
if($carip){
$sql="UPDATE parkinglot SET car='', status='' WHERE car = '$carip'";
mysql_query($sql, $conn);

$sql="Delete From car Where car_ip='$carip'";
mysql_query($sql, $conn);
}
else{
	echo "no car";
}


//자동차 출차하는 명령

//노드를 가져왔다고 침 1   0 
$shel= "shell/move.sh $car_ip 1 > /dev/null 2>/dev/null &";
shell_exec($shel);
echo "<script >document.location.href = 'get_ip.php';</script>";
	


?>