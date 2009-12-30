<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/society.php';

$society = new SOCIETY();

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',  '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',   $_REQUEST['scr_name']);

$form->addElement('text',   'code',       WEBPAGE::$gt['tblSocieties.code'],       'class=large');
$form->addElement('text',   'name',       WEBPAGE::$gt['tblSocieties.name'],       'class=large');
$form->addElement('select', 'zone_id',    WEBPAGE::$gt['tblSocieties.zone_id'],    (array('') + $society->zones()),    'class=large');
$form->addElement('select', 'advisor_id', WEBPAGE::$gt['tblSocieties.advisor_id'], (array('') + $society->advisors()), 'class=large');
$form->addElement('select', 'category',   WEBPAGE::$gt['tblSocieties.category'],   $society->categories(),             'class=large');

if ($form->validate())
{ 
  $values = $form->_submitValues;
 
  $extra['param']['value']['code']       = $values['code'];
  $extra['param']['value']['zone_id']    = $values['zone_id'];
  $extra['param']['value']['advisor_id'] = $values['advisor_id'];
  $extra['param']['value']['category']   = $values['category'];

  $extra['param']['label']['tblSocieties.code']       = $values['code']       ? $values['code']                  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.zone_id']    = $values['zone_id']    ? $zones[$values['zone_id']]       : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.advisor_id'] = $values['advisor_id'] ? $advisors[$values['advisor_id']] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.category']   = WEBPAGE::$gt[sprintf('tblSocieties.category.%s',$values['category'])];

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);

  $fields[]                = "s.name";
  $extra['name']['header'] = 'tblSocieties.name';
  $extra['name']['pack']   = '%s';
  $extra['name']['align']  = 'left';
    
  $fields[]                = "concat((lpad(z.id,3,'0')),'.',s.category,'.',(lpad(s.id,5,'0'))) as code";
  $extra['code']['header'] = 'tblSocieties.code';
  $extra['code']['pack']   = '%s';
  $extra['code']['align']  = 'center';

  $fields[]                = 's.id as view';
  $extra['view']['header'] = 'view';
  $extra['view']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&ref=BS.SCR.browseSocieties&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']  = 'center';

  $fields[]                = 's.id as edit';
  $extra['edit']['header'] = 'edit';
  $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.addSociety&ref=BS.SCR.editSociety&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
  $extra['edit']['align']  = 'center';

  if ( $values['zone_id'] )    { $operator_zone_id    = '='; } else { $operator_zone_id    = 'like'; $values['zone_id']    = '%%'; }
  if ( $values['advisor_id'] ) { $operator_advisor_id = '='; } else { $operator_advisor_id = 'like'; $values['advisor_id'] = '%%'; }
  
  if ( $values['name'] )
  {
    $operator_name  = 'like';
    $values['name'] = sprintf("%%%s%%",$values['name']);
  } else {
    $operator_name  = 'like';
    $values['name'] = '%%';
  }

  $param = sprintf("
             concat((lpad(z.id,3,'0')),'.',s.category,'.',(lpad(s.id,5,'0'))) like '%%%s%%' and
             s.name       %s '%s' and
             s.category    = '%s' and
             s.zone_id    %s '%s' and
             s.advisor_id %s '%s' and
             z.id          = s.zone_id", $values['code'],
                                         $operator_name,$values['name'],
                                         $values['category'],
                                         $operator_zone_id, $values['zone_id'],
                                         $operator_advisor_id, $values['advisor_id']);

  $mrow = SQL::select('tblSocieties AS s, tblZones AS z',$fields,$param);
  array_unshift($mrow,$extra);

  WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
  require_once 'BS.SCR.pager.inc';
  
} else {

  $form->setDefaults();
  $renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
  $renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
  $renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
  $tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
  $form->accept($renderer);
}

?>