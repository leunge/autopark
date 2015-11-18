<?php
//넣고 뺌.
	$ip =$_GET['ip'];
	$type =$_GET['type'];
	//echo $ip;
	
	if($type =='in'){
		$shel= "shell/move.sh $ip 1 > /dev/null 2>/dev/null &";
		shell_exec($shel);
		$shel= "shell/move.sh $ip 2 > /dev/null 2>/dev/null &";
		shell_exec($shel);
		$shel= "shell/move.sh $ip 0 > /dev/null 2>/dev/null &";
		shell_exec($shel);
	}else{
		$shel= "shell/move.sh $ip 1 > /dev/null 2>/dev/null &";
		shell_exec($shel);
		$shel= "shell/move.sh $ip 2 > /dev/null 2>/dev/null &";
		shell_exec($shel);
		$shel= "shell/move.sh $ip 0 > /dev/null 2>/dev/null &";
		shell_exec($shel);
	}
	
?>
moving...

