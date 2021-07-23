<?php

require_once('/opt/kwynn/kwutils.php');

doit();

function doit() {
$itlen = 64; //  + 8 + 8;

$r = popen('./a.out', 'rb');
$wr = fread($r, $itlen); kwas(strlen($wr) === $itlen, "sntp wrap fread not $itlen bytes");
pclose($r); unset($r);

decodeSNTPP($wr, 48, 'Q');
decodeSNTPP($wr, 32, 'N2');
decodeSNTPP($wr, 40, 'N2');
decodeSNTPP($wr, 56, 'Q');

exit(0);
}

function decodeSNTPP($p, $off, $unf) {
	
	static $UminusNTP = 2208988800;
	static $full32    = 4294967295;
	
	$lp = substr($p, $off, 8);
	$upn = unpack($unf, $lp);

	$un1 = $upn[1];
	
	if ($unf === 'N2') {
		$su   = $un1 - $UminusNTP;
		$fr = $upn[2] / $full32;
		$ns = $su * M_BILLION + intval(round($fr * M_BILLION));
		$ec = $ns;
	} else if ($unf === 'Q') {
		$ec = $upn[1];
	}
	
	$nf = number_format($ec);
	echo($nf);
	echo("\n");

}
