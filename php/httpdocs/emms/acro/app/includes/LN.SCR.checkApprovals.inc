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

$form->addElement('select',      'borrower_type',  WEBPAGE::$gt['tblLoansMaster.borrower_type'], ($borrower_types = array('%%'=>'') + LOAN_TYPE::borrower_types()), 'class=large');
$form->addElement('select',      'zone_id',        WEBPAGE::$gt['tblUsers.zone_id'],             ($zones = array('%%'=>'') + USER::zones()),                        'class=large');
$form->addElement('select',      'creator_id',     WEBPAGE::$gt['requestedBy'],                  ($advisors = array('%%'=>'') + CLIENT::advisors()),                'class=large');
$form->addElement('text',        'creator_date_1', WEBPAGE::$gt['date.before'],                  'class=large');
$form->addElement('text',        'creator_date_2', WEBPAGE::$gt['date.after'],                   'class=large');

$form->addRule('creator_date_1', 'yyyy-mm-dd',     'regex', '(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('creator_date_2', 'yyyy-mm-dd',     'regex', '(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');

if ($form->validate())
{ 
  $values = $form->_submitValues;

  if ($values['creator_id'] 	== '%%') { $operator_creator_id    = 'like'; } else { $operator_creator_id    = '='; }
  if ($values['borrower_type'] 	== '%%') { $operator_borrower_type = 'like'; } else { $operator_borrower_type = '='; }
  if ($values['zone_id']        == '%%') { $operator_zone_id       = 'like'; } else { $operator_zone_id       = '='; }
  
  if ($values['creator_date_1'] == '')   { $values['creator_date_1'] = '9999-01-01'; }
  if ($values['creator_date_2'] == '')   { $values['creator_date_2'] = '1000-01-01'; }

  $extra['amount']['header'] = 'amount_short';
  $extra['amount']['pack']   = '%s';
  $extra['amount']['align']  = 'right';
  
  $extra['borrower']['header']   = 'tblLoansMaster.borrower_id';
  $extra['borrower']['pack']     = '%s';
  $extra['borrower']['align']    = 'left';

  $extra['creator_id']['header'] = 'requestedBy';
  $extra['creator_id']['pack']   = '%s';
  $extra['creator_id']['align']  = 'left';

  $extra['program_id']['header'] = 'tblPrograms.program';
  $extra['program_id']['pack']   = '%s';
  $extra['program_id']['align']  = 'left';

  $extra['xp_delivered_date']['header'] = 'release';
  $extra['xp_delivered_date']['pack']   = '%s';
  $extra['xp_delivered_date']['align']  = 'right';
  
  $extra['approval']['header'] = 'doApproval';
  $extra['approval']['pack']   = sprintf(WEBPAGE::_HOTLINK_POPUP,WEBPAGE::$gt['LN.SCR.doCheckApproval'],'LN.SCR.doCheckApproval','id','%s','LN.SCR.doCheckApproval','no','yes','no','560','600',WEBPAGE::$gt['doApproval']);
  $extra['approval']['align']  = 'center';

  $extra['param']['value']['borrower_type']  = $values['borrower_type'];
  $extra['param']['value']['zone_id']        = $values['zone_id'];
  $extra['param']['value']['creator_id']     = $values['creator_id'];
  $extra['param']['value']['creator_date_1'] = $values['creator_date_1'];
  $extra['param']['value']['creator_date_2'] = $values['creator_date_2'];

  $extra['param']['label']['tblLoanTypes.borrower_type'] = $borrower_types[$values['borrower_type']] ? $borrower_types[$values['borrower_type']] : WEBPAGE::$gt['all'];
  $extra['param']['label']['tblUsers.zone_id']           = $zones[$values['zone_id']]                ? $zones[$values['zone_id']]                : WEBPAGE::$gt['all'];
  $extra['param']['label']['requestedBy']                = $advisors[$values['creator_id']]          ? $advisors[$values['creator_id']]          : WEBPAGE::$gt['all'];
  $extra['param']['label']['date.before']                = $values['creator_date_1'];
  $extra['param']['label']['date.after']                 = $values['creator_date_2'];

  $extra['param']['ref'] = WEBPAGE::$scr_name;
  $extra['param']['ts']  = microtime(true);
  
  $mid = WEBPAGE::$dbh->getAssoc("
            select
              lmdO.master_id, 1
            from
              tblLoansMasterDetails lmdO
            where
              lmdO.master_id not in
              (
                select
                  lmd.master_id
                from
                  tblLoansMasterDetails lmd
                where
                  lmd.loan_id not in (select l.id from tblLoans l where l.status = 'S')
              )
            group by
              lmdO.master_id"); 

  $mrow = WEBPAGE::$dbh->getAll(sprintf("
            select
              lm.amount amount,
              if((lm.borrower_type != 'I'), s.name, concat(c.first,' ',c.last)) borrower,
              concat(left(u.first,1),'. ',u.last) creator_id,
              concat(z.short_name,': ',p.program) program_id,
              lm.xp_delivered_date,
              lm.id approval
            from
            (
              tblLoansMaster lm,
              tblLoanTypes lt,
              tblUsers u,
              tblZones z,
              tblPrograms p,
              tblLoansMasterDetails lmd
            )
            left join
              tblClients c on c.id = lm.borrower_id
            left join
              tblSocieties s on s.id = lm.borrower_id
            where
              lm.creator_id    %s '%s'  and
              lt.borrower_type %s '%s'  and
              lm.zone_id       %s '%s'  and
              lm.creator_date  <= '%s'  and
              lm.creator_date  >= '%s'  and
              lm.creator_id     = u.id  and
              lm.zone_id        = z.id  and
              lm.program_id     = p.id  and
              lm.loan_type_id   = lt.id and
              lmd.master_id     = lm.id and
              lmd.master_id in (%s)
            group by
              lmd.master_id", $operator_creator_id, $values['creator_id'],
                              $operator_borrower_type, $values['borrower_type'],
                              $operator_zone_id, $values['zone_id'],
                              $values['creator_date_1'],
                              $values['creator_date_2'],
                              implode(',',array_keys($mid)))); 

  $mrow = is_array($mrow) ? $mrow : array();
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

