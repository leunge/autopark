<?php

//������ �Ϸ�� ��⿡�� �����Ͽ� �����Ǿ��ٰ� ǥ��. db�� �������� ����.
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