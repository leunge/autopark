<?php
//현재 내 차의 주차 현황을 보여줌.
$myip=$_SERVER['REMOTE_ADDR'];
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);
/*
if($db)
 echo "db 연결성공";
else
 echo "db 연결 실패";

*/

//내 아이디에 맞는 no받아오기.
$sql = "select park_no from car where phone_id='$myip'";
$res = mysql_query($sql, $conn);
$parkinglot = mysql_result($res, 0);

$sql = "select * from parkinglot where park_no='$parkinglot'";
//$res = mysql_query($sql, $conn);
$result = mysql_query($sql);
$res = mysql_fetch_assoc($result);

echo $parkinglot;
echo "<br>"; 
echo $res["car"]; 
echo "<br>";
echo $res["status"]; 
?>
<br>
<iframe width="700" height="550" src="http://192.168.1.1:8080/?action=snapshot"></iframe>