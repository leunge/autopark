<?php
//�ڵ����� �����ǿ� ���� �����Ǹ� �޾Ƽ� ���� �� �ڵ������� ���� ��� ����.

$myip=$_GET['myip'];
$car_ip =$_GET['carip'];

$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);

if(!$db)
 echo "db ���� ����<br>";

if($myip || $car_ip){
	//���ڸ� ã��.
	$sql = "select park_no from parkinglot where car='' limit 1";
	$res = mysql_query($sql, $conn);

	$parkinglot = mysql_result($res, 0);
	//db�� �ø���
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


//�ڵ��� �����ϴ� ��ɾ�.

//��带 �����Դٰ� ħ 1   0 
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