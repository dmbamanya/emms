<?php
require_once 'HTML/QuickForm.php';
require_once 'HTML/QuickForm/Renderer/ITStatic.php';
require_once './includes/LN.LIB.functions.inc';
require_once 'class/roles.php';
require_once 'class/persons.php';
require_once 'class/client.php';
require_once 'class/user.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/loan_master.php';

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static',      'submit',     '', WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static',      'reset',      '', WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static',      'cancel',     '', WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden',      'scr_name',       $_REQUEST['scr_name']);

$form->addElement('text',        'check_number',   WEBPAGE::$gt['tblLoansMaster.check_number'],  'class=large');
$form->addElement('select',      'check_status',   WEBPAGE::$gt['tblLoansMaster.check_status'],	 ($status = array('%%'=>'') + LOAN_MASTER::check_status()),         'class=large');
$form->addElement('select',      'borrower_type',  WEBPAGE::$gt['tblLoansMaster.borrower_type'], ($borrower_types = array('%%'=>'') + LOAN_TYPE::borrower_types()), 'class=large');
$form->addElement('select',      'zone_id',        WEBPAGE::$gt['tblUsers.zone_id'],             ($zones = array('%%'=>'') + USER::zones()),                        'class=large');
$form->addElement('select',      'creator_id',     WEBPAGE::$gt['requestedBy'],                  ($advisors = array('%%'=>'') + CLIENT::advisors()),                'class=large');
$form->addElement('text',        'creator_date_1', WEBPAGE::$gt['date.before'],                  'class=large');
$form->addElement('text',        'creator_date_2', WEBPAGE::$gt['date.after'],                   'class=large');

$form->addRule('creator_date_1', 'yyyy-mm-dd',     'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('creator_date_2', 'yyyy-mm-dd',     'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate())
{ 
  $values = $form->_submitValues;

  if ($values['check_status']   == '%%') { $operator_check_status  = 'like'; } else { $operator_check_status  = '='; }
  if ($values['creator_id']     == '%%') { $operator_creator_id    = 'like'; } else { $operator_creator_id    = '='; }
  if ($values['borrower_type']  == '%%') { $operator_borrower_type = 'like'; } else { $operator_borrower_type = '='; }
  if ($values['zone_id']        == '%%') { $operator_zone_id       = 'like'; } else { $operator_zone_id       = '='; }
  
  if ($values['creator_date_1'] == '')   { $creator_date_1         = '9999-01-01'; }
  if ($values['creator_date_2'] == '')   { $creator_date_2         = '1000-01-01'; }

  $fields[]                        = 'if(lm.check_number,lm.check_number,"-") check_number';
  $extra['check_number']['header'] = 'tblLoansMaster.check_number';
  $extra['check_number']['pack']   = '%s';
  $extra['check_number']['align']  = 'center';

  $fields[]                        = 'concat(u.first," ",u.last) creator_id';
  $extra['creator_id']['header']   = 'requestedBy';
  $extra['creator_id']['pack']     = '%s';
  $extra['creator_id']['align']    = 'left';

  $fields[]                        = 'z.zone zone_id';
  $extra['zone_id']['header']      = 'tblUsers.zone_id';
  $extra['zone_id']['pack']        = '%s';
  $extra['zone_id']['align']       = 'left';

  $fields[]                        = 'p.program program_id';
  $extra['program_id']['header']   = 'tblPrograms.program';
  $extra['program_id']['pack']     = '%s';
  $extra['program_id']['align']    = 'left';

  $fields[]                        = 'lm.id view';
  $extra['view']['header']         = 'view';
  $extra['view']['pack']           = sprintf("<a href='index.php?scr_name=LN.SCR.viewLoanMaster&id=%s'>%s</a>",'%s',WEBPAGE::$gt['view']);
  $extra['view']['align']          = 'center';
  
  $extra['param']['value']['check_number']   = $values['check_number'];
  $extra['param']['value']['check_status']   = $values['check_status'];
  $extra['param']['value']['borrower_type']  = $values['borrower_type'];
  $extra['param']['value']['zone_id']        = $values['zone_id'];
  $extra['param']['value']['creator_id']     = $values['creator_id'];
  $extra['param']['value']['creator_date_1'] = $values['creator_date_1'];
  $extra['param']['value']['creator_date_1'] = $values['creator_date_2'];

  $extra['param']['label']['tblLoansMaster.check_number'] = $values['check_number']                   ? $values['check_number']                   : '-';
  $extra['param']['label']['tblLoansMaster.check_status'] = $status[$values['check_status']]          ? $status[$values['check_status']]          : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblLoanTypes.borrower_type']  = $borrower_types[$values['borrower_type']] ? $borrower_types[$values['borrower_type']] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblUsers.zone_id']            = $zones[$values['zone_id']]                ? $zones[$values['zone_id']]                : WEBPAGE::$gt['all'];
  $extra['param']['label']['requestedBy']                 = $advisors[$values['creator_id']]          ? $advisors[$values['creator_id']]          : WEBPAGE::$gt['all'];
  $extra['param']['label']['date.before']                 = $creator_date_1;
  $extra['param']['label']['date.after']                  = $creator_date_2;

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);
  
  $tables = "tblLoansMaster lm, tblLoanTypes lt, tblUsers u, tblZones z, tblPrograms p";
  $params = sprintf("
              lm.check_number  like '%%%s%%' and
              lm.check_status  %s   '%s'     and
              lm.creator_id    %s   '%s'     and
              lt.borrower_type %s   '%s'     and
              lm.zone_id       %s   '%s'     and
              lm.creator_date  <=   '%s'     and
              lm.creator_date  >=   '%s'     and
              lm.creator_id     =   u.id     and
              lm.zone_id        =   z.id     and
              lm.program_id     =   p.id     and
              lm.loan_type_id   =   lt.id", $values['check_number'],
                                            $operator_check_status,  $values['check_status'],
                                            $operator_creator_id,    $values['creator_id'],
                                            $operator_borrower_type, $values['borrower_type'],
                                            $operator_zone_id,       $values['zone_id'],
                                            $creator_date_1,
                                            $creator_date_2);

  $mrow = SQL::select($tables,$fields,$params);
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
