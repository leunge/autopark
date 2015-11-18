<?php

//주차가 완료된 기기에서 접속하여 주차되었다고 표시. db에 주차상태 저장.
$park_no = $_GET['no'];
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);
if($db)
 echo "1";
else
 echo "0";

if($park_no){
	$sql="UPDATE parkinglot SET status='parked' WHERE park_no = '$park_no'";
	mysql_query($sql, $conn);
}else{
	echo "0";
}
?>