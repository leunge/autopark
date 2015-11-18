
<?php
//내 자동차 선택 
//$shel= "curl 192.168.1.1/con.txt | awk '{print $3, $4 ". "\"<br>\"". "}' | awk '$2 ~ /PC/'";
//$data = shell_exec($shel);
//echo "$data";

//$shel= "curl 192.168.1.1/con.txt |grep '[*] [*]' | awk '{print ". "\" {ip :\"". "$3 ". "\", name :\"". " $4". "\"}<br>\"". "}' ";

$shel= "curl 192.168.1.1/con.txt |grep '*' | awk '{print $3 ". "\", \"". " $4". "\"<br>,\"". "}' ";
$data = shell_exec($shel);

$strTok =explode(',' , $data);

//짝수가 ip 홀수가 name 
?>


<html>
<head>

</head>
<body>
choose your car<br>
<table class="general">
			<tr class="head">
				<th>No</th>
				<th>car_ip</th>
				<th>phone_ip</th>
				<th>status</th>
			</tr>
<?php
	//FOR문 돌려서 아이피와 이름을 보여줌.
	for($i = 0 ; $i < 5 ; $i ++){
			echo "<tr><td><a href='test2.php?ip=".$strTok[$i]."'><button type='button'>My Car</button></a></td>";
			echo "<td> IP : ". $strTok[$i++];
			echo "</td><td> Name : ". $strTok[$i];
			echo "</td></tr>"
	}
	//선택시 깜빡이게 함.
	//페이지 변경. IP 전송.
?>
</table>
</body>
</html>