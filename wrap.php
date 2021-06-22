<?php

require_once('/opt/kwynn/kwutils.php');
// require_once('./../sntp/sntp.php');

doit();

function doit() {

$count = 1;
$itlen = 48; //  + 8 + 8;
$totlen = $itlen * $count;

echo('1234567890123456789012345678901234567890123456789012345678901234' . "\n");
for($j=0; $j < 5; $j++) {
for($i=0; $i < $count; $i++) {
$r = popen('./sntp', 'rb');
$wr = fread($r, $totlen); kwas(strlen($wr) === $totlen, "sntp wrap fread not $totlen bytes");
pclose($r); unset($r);


 mytest(substr($wr, 0, 48));

}

// sleep(1);

}
exit(0);
}

function mystrbin2hex($bin) {
	
	$b = sprintf('%064s', $bin); unset($bin);

	$sz = strlen($b);
	kwas ($sz % 4 === 0, 'mybin2hex must be modulus 4 === 0');
	$cs = intval(round($sz / 4));

	$h = '';
	for($i=0; $i < $cs; $i++) {
		$s  = substr($b, $i * 4, 4);
		$hr = dechex(bindec($s));
		$h .= $hr;
	}

	return $h;
}

function myunpack($s) {
	$l = strlen($s);
	$b = '';
	for($i=0; $i < $l; $i++)
	for($j=7; $j >= 0; $j--) 
	{
		$c = $s[$i];
		$b .= (ord($c) & (1 << $j)) ? '1' : '0';
	}

	echo($b . "\n");
}

function mytest($p) {
	$lp = substr($p, 40, 8);

	if (0) {
	myunpack($lp);

	return; // *****
	}

	$upn = unpack('N2', $lp);
	$d10 = $upn[1];

	if (0) echo($d10);

	// if (1) echo($d10 >> 32);

	$b10 = decbin($d10);

	if (0) echo(mystrbin2hex($b10));

	$f10 = sprintf('%064s %016s %d', $b10, mystrbin2hex($b10), $d10);
	if (1) echo($f10);

	if (0) echo($upn[2]);

	if(0) {
	$d = $upn[1];
	echo($upn[1] . ' ' . $upn[2] . "\n");		
	}


	if (0) {
		$s10 = $d10; //  >> 32;
		echo($s10 . "\n");

		if (0) {$u20 = unpack('V', $s10);
			$d20 = $u20[1];
		echo(decbin($d20) . "\n");}

	}

	echo("\n");

}
