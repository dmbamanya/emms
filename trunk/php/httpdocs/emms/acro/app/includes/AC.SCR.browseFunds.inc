<?php
require_once 'class/fund.php';

$fund = new FUND();

$fields[]                = 'code';
$extra['code']['header'] = 'tblFunds.code';
$extra['code']['pack']   = '%s';
$extra['code']['align']  = 'left';
    
$fields[]                = 'fund';
$extra['fund']['header'] = 'tblFunds.fund';
$extra['fund']['pack']   = '%s';
$extra['fund']['align']  = 'left';

$fields[]                = 'id as view';
$extra['view']['header'] = 'view';
$extra['view']['pack']   = sprintf("<a href='%s?scr_name=AC.SCR.viewFund&ref=AC.SCR.browseFunds&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
$extra['view']['align']  = 'center';

$fields[]                = 'id as edit';
$extra['edit']['header'] = 'edit';
$extra['edit']['pack']   = sprintf("<a href='%s?scr_name=AC.SCR.addFund&ref=AC.SCR.editFunds&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
$extra['edit']['align']  = 'center';

$extra['param']['ref']   = WEBPAGE::$scr_name;
$extra['param']['ts']    = microtime('true');

$mrow = SQL::select('tblFunds',$fields);
array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once 'BS.SCR.pager.inc';
  
?>