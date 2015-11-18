<?php
//현재 주차된 자동차 현황을 보여줌.
$conn=mysql_connect('localhost', 'root', 'gpon5200kr');     
$db=mysql_select_db("park", $conn);
/*
if($db)
 echo "db 연결성공";
else
 echo "db 연결 실패";
*/
$sql = "select car.park_no, car, status, car_ip, phone_id from parkinglot join car on parkinglot.park_no = car.park_no";
$res = mysql_query($sql, $conn);


//select * from parkinglot join car on parkinglot.park_no = car.park_no;
?>
<!DOCTYPE html>
<html>
<head>
	<title>Admin Page</title>
	<link rel="stylesheet" type="text/css" href="css/table-general.css" />
    <style type="text/css">
	body {
			font-family: Arial, Verdana, sans-serif;
			font-size: 90%;
			color: #666;
			margin-top:100px;
			background-color: #f8f8f8;}
	</style>
</head>

<body>
	<center>
		<h1>Parking Status</h1>
		<br>
		<table class="general">
			<tr class="head">
				<th>No</th>
				<th>car_ip</th>
				<th>phone_ip</th>
				<th>status</th>
			</tr>
			<?php
				$i=1;
				while($row= mysql_fetch_array($res)){
					if($i%2 == 0){
						echo "<tr>";
						$i = 1;
					}else{
						echo "<tr class='even'>";
						$i = 2;
					}
					echo "<td>".$row[park_no]."</td><td>".$row[car_ip]."</td><td>".$row[phone_id]."</td><td>".$row[status]."</td>";
					echo "</tr>";
				}
			?>
			
		</table>
	</center>
</body>
</html>
