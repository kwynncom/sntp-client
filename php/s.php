#! /usr/bin/php
<?php

require_once('/opt/kwynn/kwutils.php');

class sntp_wrapper {
	
	const cmdBase = 'sntp -nosleep '; // I am sleeping in PHP; if I do it in the shell_exec, weird things happen
	const maxNISTS = 4;
	
	public function __construct() {
		$this->setCmd();
		$t = shell_exec($this->cmdo);
		echo($t);
		$a = $this->getValid($t);
		if (!$a) {
			echo('invalid result' . "\n");
			exit(9382);
		}
		$min = min($a);
		foreach($a as $n) echo(number_format($n - $min) . "\n");
		echo(number_format(self::SNTPOffset($a)) . "\n");
		echo('Results **OK**' . "\n");
		$this->maybeSleep();
	}
	
	private function maybeSleep() {
		if (!$this->dosleep) return;
		sleep(self::maxNISTS);		
	}
		
	public static function SNTPOffset($T) {
		$t = ((($T[1] - $T[0]) + ($T[2] - $T[3]))) >> 1;
		return $t;
	}
	
	private function setCmd() {
		global $argc, $argv;
				
		$c  = self::cmdBase;
		$c .= 'sntp -nosleep ';
		if ($argc > 1) $c .= implode(' ', array_slice($argv, 1));	
		$this->cmdo = $c;
		
		$this->dosleep = true;		
		if ($argc <= 1) return;
		for ($i=1; $i < $argc; $i++) if ($argv[$i] === '-nosleep') { $this->dosleep = false; break; }
	}
	
	public static function getValid($t) {
		try {
			$a = explode("\n", trim($t)); unset($t); kwas(count($a) === 5, 'wrong lines sntp sanity check'); unset($a[4]);

			for ($i=0; $i < 4; $i++) $a[$i] = intval($a[$i]);
			$min = min($a);
			$max = max($a);
			kwas($max - $min < M_BILLION, 'time sanity check fails - ck 1 0417');
			$ds = abs(nanotime() - $max);
			kwas($ds < M_BILLION , 'time sanity check fail 2');
			kwas($a[1] <= $a[2], 'server time sanity check fail between in and out');
			kwas($a[0] <  $a[3], 'server time sanity check internal out and in');

			return $a;
		} catch(Exception $ex) { }

		return [];
	}
	
}

new sntp_wrapper();
