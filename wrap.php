<?php

require_once('/opt/kwynn/kwutils.php');

class sntpWrapper {
	
	const bin	    = './sntp';
	const expectLen = 64;

	function __construct() {
		$dat = $this->get();
		$ns = $this->parse($dat); unset($dat);
		$this->out($ns); unset($ns);
		
	}
	
	private function out($ns) {
		foreach($ns as $ec) {
			$nf = number_format($ec);
			echo($nf);
			echo("\n");		
		}
	}
	
	private function get() {
		$r = popen(self::bin, 'rb');
		$wr = fread($r, self::expectLen); kwas(strlen($wr) === self::expectLen, 'sntp wrap fread not ' . self::expectLen . 'bytes');
		pclose($r); unset($r);
		return $wr;
	}
	
	private function parse($wr) {
		$ns[] = self::decodeSNTPP($wr, 48, 'Q');
		$ns[] = self::decodeSNTPP($wr, 32, 'N2');
		$ns[] = self::decodeSNTPP($wr, 40, 'N2');
		$ns[] = self::decodeSNTPP($wr, 56, 'Q');
		return $ns;
	}

	private static function decodeSNTPP($p, $off, $unf) {

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
		} else if ($unf === 'Q') $ec = $upn[1];
		
		return $ec;
	}
} // class

new sntpWrapper();
