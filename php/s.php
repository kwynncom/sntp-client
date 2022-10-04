#! /usr/bin/php
<?php

require_once('/opt/kwynn/kwutils.php');

class sntp_wrapper {
	
	const cmdBase = 'sntp -nosleep '; // I am sleeping in PHP; if I do it in the shell_exec, weird things happen
	const maxNISTS = 4;
	const ipi      = 4;
	const tlines   = 4;
	const tchars   = 79;
	
	private function procArgs() {
		global $argv, $argc;
	
		$this->json = false;
		for($i = 1; $i < $argc; $i++) if ($argv[$i] === '-json') { $this->json = true; break; }
	}
	
	public function __construct() {
		
		$this->json = true;
		
		try {
			$this->procArgs();
			$js = $this->json;
			$this->setCmd();
			$t = shell_exec($this->cmdo);
			if (!$js) echo($t);
			$a = $this->popValid($t);

			if (!$js) { 
				$min = min($a);
				foreach($a as $n) echo(number_format($n - $min) . "\n");
				echo(number_format(self::SNTPOffset($a)) . " = offset\n");
				echo(number_format($a[1] - $a[0]) . " = out\n");
				echo(number_format($a[3] - $a[2]) . " = in\n" );
				echo('Results **OK**' . "\n");
			}
			
			$this->maybeSleep();
			
		} catch(Exception $ex) {};
		
		$this->json();
	}
	
	private function json() {
		
		if (!$this->json) return;
		
		$ret['status'] = false;
		if (isset($this->ot4)) {
			$ret['status'] = 'OK';
			$ret['t4'] = $this->ot4;
		}
		
		$ret['ip'] = $this->oip;
		
		kwjae($ret);
	}
	
	private function maybeSleep() {
		if (!$this->dosleep) return;
		if (!$this->json) echo('PHP sleeping.  Zzzz....' . "\n");
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
	
	private function popValidTs($a) {
		$n = self::tlines;
		kwas(count($a) === $n, 'bad tline count sntp sanity 2');
		for ($i=0; $i < $n; $i++) $a[$i] = intval($a[$i]);
		kwas(count($a) >= 4, 'fail - for immediate tline sntp sanity purposes');
		
		$min = min($a);
		$max = max($a);
		kwas($max - $min < M_BILLION, 'time sanity check fails - ck 1 0417');
		$ds = abs(nanotime() - $max);
		kwas($ds < M_BILLION , 'time sanity check fail 2');
		kwas($a[1] <= $a[2], 'server time sanity check fail between in and out');
		kwas($a[0] <  $a[3], 'server time sanity check internal out and in');
		$this->ot4 = $a;
		return $a;
	}
	
	private function popValid($t) {
			
		$a = explode("\n", trim($t)); unset($t); kwas(count($a) >= self::tlines, 'wrong lines sntp sanity check'); 
		$this->oip = getValidIPOrFalsey(kwifs($a, self::ipi)); 
		return $this->popValidTs(array_slice($a, 0, self::tlines));

	}
	
}

new sntp_wrapper();
