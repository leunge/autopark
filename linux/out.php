<?php
//�ڵ����� �����ǿ� ���� �����Ǹ� �޾Ƽ� ���� �� �ڵ������� ���� ��� ����.

$carip=$_GET['carip'];
echo $carip;
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);

if($db)
 echo "db ���Ἲ��<br>";
else
 echo "db ���� ����<br>";


//db���� ����.
if($carip){
$sql="UPDATE parkinglot SET car='', status='' WHERE car = '$carip'";
mysql_query($sql, $conn);

$sql="Delete From car Where car_ip='$carip'";
mysql_query($sql, $conn);
}
else{
	echo "no car";
}


//�ڵ��� �����ϴ� ���

//��带 �����Դٰ� ħ 1   0 
$shel= "shell/move.sh $car_ip 1 > /dev/null 2>/dev/null &";
shell_exec($shel);
echo "<script >document.location.href = 'get_ip.php';</script>";
	


?>