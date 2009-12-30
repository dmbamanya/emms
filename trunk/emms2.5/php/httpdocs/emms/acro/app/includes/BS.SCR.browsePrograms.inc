<?php
require_once 'class/program.php';

$program = new PROGRAM();

$fields[]                   = 'program';
$extra['program']['header'] = 'tblPrograms.program';
$extra['program']['pack']   = '%s';
$extra['program']['align']  = 'left';

$fields[]                   = 'id as view';
$extra['view']['header']    = 'view';
$extra['view']['pack']      = sprintf("<a href='%s?scr_name=BS.SCR.viewProgram&ref=BS.SCR.browsePrograms&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
$extra['view']['align']     = 'center';

$fields[]                   = 'id as edit';
$extra['edit']['header']    = 'edit';
$extra['edit']['pack']      = sprintf("<a href='%s?scr_name=BS.SCR.addProgram&ref=BS.SCR.editProgram&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
$extra['edit']['align']     = 'center';

$extra['param']['ref']      = WEBPAGE::$scr_name;
$extra['param']['ts']       = microtime(true);
 
$mrow = SQL::select('tblPrograms',$fields);
array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once 'BS.SCR.pager.inc';
  
?>