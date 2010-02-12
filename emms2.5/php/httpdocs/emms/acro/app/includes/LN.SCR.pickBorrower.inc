<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once 'class/roles.php';
require_once 'class/society.php';
require_once 'class/loan_type.php';

$ltype   = new LOAN_TYPE();
$society = new SOCIETY();

$form    = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit',    '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset',     '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel',    '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',      $_REQUEST['scr_name']);


$form->addElement('text',   'code',          WEBPAGE::$gt['tblSocieties.code'], 'class=large');
$form->addElement('text',   'name',          WEBPAGE::$gt['tblSocieties.name'], 'class=large');

$form->addElement('select', 'borrower_type', WEBPAGE::$gt['tblLoanTypes.borrower_type'], (array(''=>'') + $ltype->borrower_types()),           'class=large');
$form->addElement('select', 'zone_id',       WEBPAGE::$gt['tblSocieties.zone_id'],       ($zones = (array(''=>'') + $society->zones())),       'class=large');
$form->addElement('select', 'advisor_id',    WEBPAGE::$gt['tblSocieties.advisor_id'],    ($advisors = (array(''=>'') + $society->advisors())), 'class=large');

$form->addRule('borrower_type', WEBPAGE::$gt['selectOption'],'required');
$form->addRule('borrower_type', WEBPAGE::$gt['selectOption'],'alphanumeric');
$form->addRule('zone_id',       WEBPAGE::$gt['selectOption'],'required');
$form->addRule('zone_id',       WEBPAGE::$gt['selectOption'],'numeric');
$form->addRule('advisor_id',    WEBPAGE::$gt['selectOption'],'required');
$form->addRule('advisor_id',    WEBPAGE::$gt['selectOption'],'numeric');

if ($form->validate())
{ 
  $values = $form->_submitValues;
  
  $extra['param']['value']['name']          = $values['name'];
  $extra['param']['value']['code']          = $values['code'];
  $extra['param']['value']['zone_id']       = $values['zone_id'];
  $extra['param']['value']['advisor_id']    = $values['advisor_id'];
  $extra['param']['value']['borrower_type'] = $values['borrower_type'];

  $extra['param']['label']['tblSocieties.name']          = $values['name']       ? $values['name']                  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.code']          = $values['code']       ? $values['code']                  : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.zone_id']       = $values['zone_id']    ? $zones[$values['zone_id']]       : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblSocieties.advisor_id']    = $values['advisor_id'] ? $advisors[$values['advisor_id']] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblLoanTypes.borrower_type'] = WEBPAGE::$gt[sprintf('tblLoanTypes.borrower_type.%s',$values['borrower_type'])];
    
  $extra['param']['ref']   = 'LN.SCR.requestLoan';
  $extra['param']['ts']    = microtime(true);

  $fields[]                = "name";
  $extra['name']['header'] = 'tblSocieties.name';
  $extra['name']['pack']   = '%s';
  $extra['name']['align']  = 'left';
    
  $fields[]                = "code";
  $extra['code']['header'] = 'tblSocieties.code';
  $extra['code']['pack']   = '%s';
  $extra['code']['align']  = 'center';

  
  switch ($values['borrower_type'])
  {
    case 'B':
      $fields[]                = 'id as view';
      $extra['view']['header'] = 'view';
      $extra['view']['pack']   = sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['tips']['view_society_profile'],'BS.SCR.viewSocietyXpress','id','%s','BS.SCR.viewSocietyXpress','no','no','no','400','500',WEBPAGE::$gt['view']);
      $extra['view']['align']  = 'center';

      $fields[]                = 'id as edit';
      $extra['edit']['header'] = 'edit';
      $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
      $extra['edit']['align']  = 'center';

      $fields[]                       = 'id as requestLoan';
      $extra['requestLoan']['header'] = 'requestLoan';
      $extra['requestLoan']['pack']   = sprintf("<a href='%s?scr_name=LN.SCR.requestLoan&bt=B&bid=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['requestLoan']);
      $extra['requestLoan']['align']  = 'center';

      $param = sprintf("code like '%%%s%%'", $values['code']);
      $table = sprintf("(select ss.id, ss.name, concat(lpad(ss.zone_id,3,'0'),'.',ss.category,'.',lpad(ss.id,5,'0')) code from tblSocieties ss where ss.category = 'B' and ss.zone_id = '%s' and ss.advisor_id='%s' and ss.name like '%%%s%%') view", $values['zone_id'], $values['advisor_id'], $values['name']);
      break;

    case 'G':
      $fields[]                = 'id as view';
      $extra['view']['header'] = 'view';
      $extra['view']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&ref=BS.SCR.browseSocieties&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
      $extra['view']['align']  = 'center';

      $fields[]                = 'id as edit';
      $extra['edit']['header'] = 'edit';
      $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewSociety&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
      $extra['edit']['align']  = 'center';

      $fields[]                       = 'id as requestLoan';
      $extra['requestLoan']['header'] = 'requestLoan';
      $extra['requestLoan']['pack']   = sprintf("<a href='%s?scr_name=LN.SCR.requestLoan&bt=G&bid=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['requestLoan']);
      $extra['requestLoan']['align']  = 'center';

      $param = sprintf("code like '%%%s%%'", $values['code']);
      $table = sprintf("(select ss.id, ss.name, concat(LPAD(ss.zone_id,3,'0'),'.',ss.category,'.',lpad(ss.id,5,'0')) code from tblSocieties ss where ss.category = 'G' and ss.zone_id = '%s' and ss.advisor_id='%s' and ss.name like '%%%s%%') view", $values['zone_id'], $values['advisor_id'], $values['name']);
      break;

    case 'I':
      $fields[]                = 'id as view';
      $extra['view']['header'] = 'view';
      $extra['view']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewClient&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['view']);
      $extra['view']['align']  = 'center';

      $fields[]                = 'id as edit';
      $extra['edit']['header'] = 'edit';
      $extra['edit']['pack']   = sprintf("<a href='%s?scr_name=BS.SCR.viewClient&id=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['edit']);
      $extra['edit']['align']  = 'center';

      $fields[]                       = 'id as requestLoan';
      $extra['requestLoan']['header'] = 'requestLoan';
      $extra['requestLoan']['pack']   = sprintf("<a href='%s?scr_name=LN.SCR.requestLoan&bt=I&bid=%s'>%s</a>", $PHP_SELF,'%s',WEBPAGE::$gt['requestLoan']);
      $extra['requestLoan']['align']  = 'center';

      $param = sprintf("name like '%%%s%%'", $values['name']);
      $table = sprintf("(select ss.id, ss.code, concat(first,' ',middle,' ',last) name from tblClients ss where ss.society_id = 0 and ss.zone_id = '%s' and ss.advisor_id='%s' and ss.code like '%%%s%%') view", $values['zone_id'], $values['advisor_id'], $values['code']);
      break;

    default:
      break;
  }
 
  $mrow = SQL::select($table,$fields,$param);
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
