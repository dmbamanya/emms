<?php
require_once 'Pager/Pager.php';
require_once 'HTML/QuickForm.php';

if (!is_array($mrow)) 	{ $mrow = WEBPAGE::getCacheData(WEBPAGE::$queryCache.'.'.$_REQUEST['ts']); }

$extra = array_shift($mrow);
$referrer['ref'] = $extra['param']['ref'];
$referrer['ts']  = $extra['param']['ts'];
$popup = $extra['param']['popup'];

if (!$popup) {  // refresh results feature
  $form = new HTML_QuickForm('searchForm','POST');
  $form->addElement('hidden','scr_name',$referrer['ref']);
  if (is_array($extra['param']['value'])) {
    foreach ($extra['param']['value'] as $key=>$val) {
      $form->addElement('hidden',$key,$val);
      }
    $tpl->setVariable('new_search',WEBPAGE::$btn->imglink(sprintf("%s?scr_name=%s",$PHP_SELF,$referrer['ref']),WEBPAGE::$gt['newSearch']));
    $tpl->setVariable('refresh_button',WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['refresh']));
    $tpl->setVariable('refresh_form',$form->toHtml());
    }
  $form->freeze();
  }
  
$tpl->setVariable('content_title',WEBPAGE::$gt[$referrer['ref']]);

$tpl->replaceBlockfile('content',sprintf("%s.%s",WEBPAGE::_PAGER_EXTRA_SCR_NAME,'tpl'));

$pager = & Pager::factory(WEBPAGE::getPagerOptions($referrer,$mrow,$items_perpage));

$data  = $pager->getPageData();
$links = $pager->getLinks(); 

if (is_array($extra['param']['label'])) {
  foreach ($extra['param']['label'] as $key=>$val) {
    if ($val != WEBPAGE::$gt['all']) { 
      $tpl->setCurrentBlock("filter");
      $tpl->setVariable("criteria_label", WEBPAGE::$gt[$key]) ;
      $tpl->setVariable("criteria_value", $val) ;
      $tpl->parseCurrentBlock("filter");
      }
    }
  } 
  
unset($extra['param']);

if (!count($mrow)) {

  $total = 0;
  
  } else {
  
  $tpl->setCurrentBlock("header");
  $tpl->setVariable("column_name", '#') ;
  $tpl->parseCurrentBlock("header");

  foreach ($extra as $key => $val) {
    $tpl->setCurrentBlock("header");
    $tpl->setVariable("column_name", WEBPAGE::$gt[$val['header']]) ;
    $tpl->parseCurrentBlock("header");
    }

  foreach ($data as $i => $row) { 
    $tpl->setCurrentBlock("row");
    $tpl->setVariable("align", 'right') ;
    $tpl->setVariable("item", $i+1) ;
    $tpl->parseCurrentBlock("row");

    foreach ($row as $key => $val) {
      $tpl->setCurrentBlock("row");
      $tpl->setVariable("align", $extra[$key]['align']) ;
      $tpl->setVariable("item", WEBPAGE::$gt[sprintf($extra[$key]['pack'],$val)] ? WEBPAGE::$gt[sprintf($extra[$key]['pack'],$val)] : sprintf($extra[$key]['pack'],$val)) ;
      $tpl->parseCurrentBlock("row");
      }
    
	$tpl->setCurrentBlock("results");
    $tpl->parseCurrentBlock("results");
    }
  
  $total = $pager->numItems();
  } 

$tpl->setVariable('first',$links['first']);
$tpl->setVariable('back',$links['back']);
$tpl->setVariable('pages',$links['pages']);
$tpl->setVariable('next',$links['next']);
$tpl->setVariable('last',$links['last']);

$tpl->setCurrentBlock("query_results") ;
$tpl->parseCurrentBlock("query_results") ;
//$tpl->setVariable('filter_label',WEBPAGE::$gt['criteria']);
//$tpl->setVariable('no_criteria',$no_criteria);
$tpl->setVariable('total_label',WEBPAGE::$gt['results']);
$tpl->setVariable('total',$total);
$tpl->setVariable('timestamp',$referrer['ts']);
$tpl->setVariable('xls_download',WEBPAGE::$gt['RP.SCR.QueryCacheToXLS']);

?>