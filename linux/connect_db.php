<?php
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);
if($db)
 echo "db ���Ἲ��";
else
 echo "db ���� ����";

//$sql="insert into parkinglot values('','', NULL)";
//mysql_query($sql, $conn);

$sql="insert into parkinglot values('','', NULL)";
mysql_query($sql, $conn);

//select park_no from parkinglot where car='' limit 1;
//update parkinglot set car='ip', status='going' where park_no=''  
?>