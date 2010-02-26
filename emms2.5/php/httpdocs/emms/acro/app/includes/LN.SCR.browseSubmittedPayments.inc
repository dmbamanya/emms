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
require_once 'class/stdo.php';

$data = array();
parse_str($_SERVER['HTTP_REFERER'],$data);

if (array_shift($data) == 'LN.SCR.deletePayment')
{ 
  $_POST['zone_id']       = $data['zone_id'];
  $_POST['advisor_id']    = $data['advisor_id']    ? $data['advisor_id']    : '%%';
  $_POST['date']          = $data['date'];
  $_POST['borrower_type'] = $data['borrower_type'] ? $data['borrower_type'] : '%%';
  $_POST['submit']        = true;
  $tpl->setVariable('message', 	WEBPAGE::printmessage('scs',WEBPAGE::$gt['scs']));
}

$form = new HTML_QuickForm('searchForm','POST');

$form->addElement('static', 'submit', 	'',  WEBPAGE::$btn->imgsubmit('searchForm',WEBPAGE::$gt['search']));
$form->addElement('static', 'reset', 	'',  WEBPAGE::$btn->imgreset('searchForm',WEBPAGE::$gt['reset']));
$form->addElement('static', 'cancel', 	'',  WEBPAGE::$btn->imglink($PHP_SELF,WEBPAGE::$gt['cancel']));
$form->addElement('hidden', 'scr_name',      $_REQUEST['scr_name']);

$form->addElement('text',   'date',          WEBPAGE::$gt['date'], 'class=large');

$form->addElement('select', 'zone_id',       WEBPAGE::$gt['tblUsers.zone_id'],           ($zones          = array('%%'=>'') + USER::zones()),               'class=large');
$form->addElement('select', 'advisor_id',    WEBPAGE::$gt['tblClients.advisor_id'],      ($advisors       = array('%%'=>'') + CLIENT::advisors()),          'class=large');
$form->addElement('select', 'borrower_type', WEBPAGE::$gt['tblLoanTypes.borrower_type'], ($borrower_types = array('%%'=>'') + LOAN_TYPE::borrower_types()), 'class=large');

$form->addRule('date',    'yyyy-mm-dd', 'required');
$form->addRule('date',    'yyyy-mm-dd', 'regex','(^([0-9]{4}-[0-9]{2}-[0-9]{2})$)');
$form->addRule('zone_id', WEBPAGE::$gt['selectOption'],'required');
$form->addRule('zone_id', WEBPAGE::$gt['selectOption'],'numeric');

if ($form->validate())
{ 
  $values = $form->_submitValues;
  $form->freeze();
  $form->removeElement('submit');
  $form->removeElement('reset');
  $form->removeElement('cancel');

  if ($values['borrower_type'] 	== '%%') { $operator_borrower_type = 'LIKE'; $form->removeElement('borrower_type'); $borrower_type = '';} else { $operator_borrower_type = '='; $borrower_type = $values['borrower_type'];}
  if ($values['advisor_id'] 	== '%%') { $operator_advisor_id    = 'LIKE'; $form->removeElement('advisor_id'); $advisor_id = '';   }    else { $operator_advisor_id 	 = '='; $advisor_id = $values['advisor_id'];}

  $mrow = WEBPAGE::$dbh->getAll(sprintf("
            select
              p.id,
              l.loan_code,
              concat(c.first, ' ', c.last) client,
              concat((lpad(c.zone_id,3,'0')),'.',s.category,'.',(lpad(s.id,5,'0'))) groupcode,
              p.pmt,
              p.principal,
              p.insurances,
              p.fees,
              p.interest,
              p.penalties,
              p.balance_kp,
              if(p.special,'%s','%s') as special,
              p.transaction_id,
              u.username
            from
              (
                tblLoans as l,
                tblClients as c,
                tblPayments as p,
                tblLoanTypes as lt,
                tblUsers as u
              )
            left join
              tblSocieties as s on c.society_id = s.id
            where
              u.id = p.user_id         and
              p.date = '%s'            and
              l.id = p.loan_id         and
              l.zone_id = %s           and
              c.id = l.client_id       and
              l.advisor_id %s '%s'     and
              lt.borrower_type %s '%s' and
              lt.id = l.loan_type_id
            order by
              groupcode,
              client,
              p.id",
          WEBPAGE::$gt['yes'],
          WEBPAGE::$gt['no'],
          $values['date'],
          $values['zone_id'],
          $operator_advisor_id,
          $values['advisor_id'],
          $operator_borrower_type,
          $values['borrower_type']));

  $head = array
          (
            'loan_code'      => WEBPAGE::$gt['tblLoans.loan_code'],
            'client'         => WEBPAGE::$gt['tblClients.id'],
            'groupcode'      => WEBPAGE::$gt['groupcode'],
            'pmt'            => WEBPAGE::$gt['tblPayments.pmt'],
            'principal'      => WEBPAGE::$gt['tblPayments.principal'],
            'insurances'     => WEBPAGE::$gt['tblPayments.insurances'],
            'fees'           => WEBPAGE::$gt['tblPayments.fees'],
            'interest'       => WEBPAGE::$gt['tblPayments.interest'],
            'penalties'      => WEBPAGE::$gt['tblPayments.penalties'],
            'balance_kp'     => WEBPAGE::$gt['tblPayments.balance_kp'],
            'special'        => WEBPAGE::$gt['tblPayments.special'],
            'transaction_id' => WEBPAGE::$gt['tblPayments.transaction_id'],
            'username'       => WEBPAGE::$gt['tblUsers.id']
          );

  $total = array();
  foreach ($mrow as $key=>$val)
  {
    $mrow[$key]['client']         = str_replace(' ','&nbsp;',$val['client']);
    $mrow[$key]['transaction_id'] = $val['transaction_id'] ? $val['transaction_id'] : sprintf("<a href='index.popup.php?scr_name=LN.SCR.deletePayment&id=%s&zone_id=%s&advisor_id=%s&date=%s&borrower_type=%s'>%s</a>",$val['id'],$values['zone_id'],$advisor_id,$values['date'],$borrower_type,WEBPAGE::$gt['rollback']);
    $total['loan_code']           = WEBPAGE::$gt['total'];
    $total['client']              = '-';
    $total['groupcode']           = '-';
    $total['pmt']                += $val['pmt'];
    $total['principal']          += $val['principal'];
    $total['insurances']         += $val['insurances'];
    $total['fees']               += $val['fees'];
    $total['interest']           += $val['interest'];
    $total['penalties']          += $val['penalties'];
    $total['balance_kp']          = '-';
    $total['special']             = '-';
    $total['transaction_id']      = '-';
    $total['username']            = '-';
    unset($mrow[$key]['id']);    
  }
  $mrow[] = $total;

  $tpl->setVariable('chart', count($total) ? WEBPAGE::printchart($mrow,$head) : WEBPAGE::$gt['noData']);

} 

$form->setDefaults(array('date'=>date('Y-m-d')));
$renderer = new HTML_QuickForm_Renderer_ITStatic($tpl);
$renderer->setRequiredTemplate(WEBPAGE::_REQUIRED_FIELD);
$renderer->setErrorTemplate(WEBPAGE::_FIELD_ERROR);
$tpl->setVariable('content_title',WEBPAGE::$gt[WEBPAGE::$scr_name]);
$form->accept($renderer);
  
?>