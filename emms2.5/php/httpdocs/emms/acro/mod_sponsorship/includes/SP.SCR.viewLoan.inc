<?php
require_once '../app/includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/zone.php';
require_once 'class/society.php';

$master = new LOAN_MASTER($_REQUEST['id']);
if($master->isSponsor(WEBPAGE::$userID)  == 0) exit;

$master->loadextrainfo();
$master->loadBorrowerData();

$zone = new ZONE($master->data['zone_id']);

/*
 * Retrive client/loan info
 */
foreach ($master->data['loans'] as $key => $val)
{
    $loan = new LOAN($key);

    $cli = current(WEBPAGE::$dbh->getAll(sprintf
    ("
        select
            c.id,
            c.gender                                                      cli_gender,
            concat(c.first, ' ',substring(c.last,1,1))                    cli_name,
            date_format(curdate(),'%%Y') - date_format(c.birthdate,'%%Y') cli_age,
            c.education                                                   cli_education,
            c.dependants                                                  cli_dependants,
            c. creator_date                                               cli_creator_date,
            bt.description                                                cli_business
        from
            tblClients c, tblBusiness b, tblBusinessTypes bt
        where
            c.id = %s and
            b.client_list = c.id and
            bt.id = b.type_id", $loan->data['client_id']))
     );

    $payments = current(WEBPAGE::$dbh->getAll(sprintf("select sum(pmt) pmt,sum(penalties) penalties,sum(interest) interest,sum(insurances) insurances,sum(fees) fees,sum(principal) principal, min(balance_kp) balance_kp from tblPayments where loan_id = %s",$loan->data['id'])));
    $payments['balance_kp']     = $payments['balance_kp'] ? $payments['balance_kp'] : round($master->data['extrainfo'][$key]['kp'],2);


    $tpl->setCurrentBlock("bdeloans");

    $tpl->setVariable("cli_photo",        '../app/'.sprintf(WEBPAGE::_IMG_CLIENT,'I',$cli['id'])) ;
    $tpl->setVariable("cli_name",         strtoupper($cli['cli_name'])) ;
    $tpl->setVariable("cli_age",          $cli['cli_age']) ;
    $tpl->setVariable("cli_gender",       WEBPAGE::$gt[sprintf('tblClients.gender.%s',$cli['cli_gender'])]);
    $tpl->setVariable("cli_dependants",   $cli['cli_dependants']) ;
    $tpl->setVariable("cli_education",    WEBPAGE::$gt[sprintf('tblClients.education.%s',$cli['cli_education'])]) ;
    $tpl->setVariable("cli_creator_date", date_format(date_create($cli['cli_creator_date']),'M j, Y'));
    $tpl->setVariable("loan_amount",      number_format(round($master->data['extrainfo'][$key]['kp'],2), 2, '.', ','));
    $tpl->setVariable("business",         $cli['cli_business']) ;

    $tpl->setVariable('balance_kp_label', WEBPAGE::$gt['current_balance']);
    $tpl->setVariable('balance_kp',       number_format($payments['balance_kp'], 2, '.', ','));
    $tpl->setVariable('pmt_label',        WEBPAGE::$gt['principalRepayment']);
    $tpl->setVariable('pmt',              number_format($payments['pmt'], 2, '.', ','));
    $tpl->setVariable('penalties_label',  WEBPAGE::$gt['tblPayments.penalties']);
    $tpl->setVariable('penalties',        number_format($payments['penalties'], 2, '.', ','));

    $tpl->parseCurrentBlock("bdeloans");
}

$tpl->setVariable('org_name',            WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('country',             WEBPAGE::$conf['app']['install_acronym']);
$tpl->setVariable('branch',              $zone->zonedata['zone']);

$tpl->setVariable('bde_name',            strtoupper($master->data['borrower']->data['name']));
$tpl->setVariable('amount',              number_format(round($master->data['amount'],2), 2, '.', ','));
$tpl->setVariable('delivered_date',      $loan->data['delivered_date']);

?>