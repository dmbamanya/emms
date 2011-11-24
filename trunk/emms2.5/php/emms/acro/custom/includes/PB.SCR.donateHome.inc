<?php
require_once './includes/LN.LIB.functions.inc';
require_once 'class/loan_master.php';
require_once 'class/loan_type.php';
require_once 'class/loan.php';
require_once 'class/zone.php';
require_once 'class/society.php';

/*
 * Retrive currency exchange rate
 */
$xrate = current(WEBPAGE::$dbh->getAssoc(sprintf
(
  "select currency_id, rate from tblCurrenciesExchangeRates where currency_id = '%s' order by id desc limit 1",
        WEBPAGE::$conf['app']['xrate_local']))
);
$cur_code = current(WEBPAGE::$dbh->getAssoc(sprintf
(
  "select id,code from tblCurrencys where id = '%s'",
        WEBPAGE::$conf['app']['xrate_reference']))
);

$ids = LOAN_MASTER::publishable_ids();

if (count($ids) == 0) 
{ 
    header(sprintf("Location: %sindex.pub.php?scr_name=PB.SCR.donateMsgNoPubGroups", WEBPAGE::$conf['app']['url']));
    exit;
}

if (array_key_exists($_REQUEST['id'], $ids))
{
    while (key($ids) != $_REQUEST['id']) next($ids);
    if ($_REQUEST['prev'] == 1)
    {
        if (!prev($ids)) end($ids);
    }
    else
    {
        if (!next($ids)) reset($ids);
    }
}
  
$master = new LOAN_MASTER(key($ids));
$master->loadBorrowerData();

$zone = new ZONE($master->data['zone_id']);

$tpl->setVariable('min_donation',        WEBPAGE::$conf['mod_sponsorship']['min_donation']);
$tpl->setVariable('cur_code',            $cur_code);
$tpl->setVariable('lang',                WEBPAGE::$conf['mod_sponsorship']['default_language']);

$tpl->setVariable('master_id',           $master->data['id']);

$tpl->setVariable('org_name',            WEBPAGE::$conf['app']['client_name']);
$tpl->setVariable('country',             WEBPAGE::$conf['app']['install_acronym']);
$tpl->setVariable('branch',              $zone->zonedata['zone']);

$tpl->setVariable('image',               sprintf(WEBPAGE::_IMG_CLIENT,$master->data['borrower_type'],$master->data['borrower_id']));
$tpl->setVariable('bde_name',            strtoupper($master->data['borrower']->data['name']));
$tpl->setVariable('amount',              number_format(round($master->data['amount']/$xrate,2), 2, '.', ','));
$tpl->setVariable('donations',           number_format(round($ids[$master->data['id']]['donations']/$xrate,2), 2, '.', ','));
$tpl->setVariable('pending',             number_format(round($ids[$master->data['id']]['pending']/$xrate,2), 2, '.', ','));
$tpl->setVariable('fpending',            number_format(round($ids[$master->data['id']]['pending']/$xrate,2), 2, '.', ''));
$tpl->setVariable('time_left',           $ids[$master->data['id']]['time_left']);
$tpl->setVariable('progress',            100*round($ids[$master->data['id']]['donations']/$master->data['amount'],2));

?>

