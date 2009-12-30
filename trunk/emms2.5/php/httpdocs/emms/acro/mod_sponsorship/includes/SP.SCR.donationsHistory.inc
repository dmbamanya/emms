<?php

$show_gohome = true;

$extra['src_amount']['header']   = 'tblSponsorsDonations.src_amount';
$extra['src_amount']['pack']     = '%s';
$extra['src_amount']['align']    = 'right';

$extra['conv_amount']['header']  = 'tblSponsorsDonations.conv_currency_id';
$extra['conv_amount']['pack']    = '%s';
$extra['conv_amount']['align']   = 'right';

$extra['rate']['header']         = 'tblSponsorsDonations.rate';
$extra['rate']['pack']           = '%s';
$extra['rate']['align']          = 'right';

$extra['creator_date']['header'] = 'tblSponsorsDonations.creator_date';
$extra['creator_date']['pack']   = '%s';
$extra['creator_date']['align']  = 'right';

$extra['param']['ref']           = WEBPAGE::$scr_name;
$extra['param']['ts']            = microtime(true);

$mrow = WEBPAGE::$dbh->getAll(sprintf("
          select
            concat(cs.symbol,' ',sd.src_amount) src_amount,
            concat(cc.symbol,' ',sd.conv_amount) conv_amount,
            round(sd.conv_amount/sd.src_amount,2) rate,
            sd.creator_date
          from
            tblSponsorsDonations sd,
            tblCurrencys cs,
            tblCurrencys cc
          where
            sd.sponsor_id = '%s'               and
            cs.id         = sd.src_currency_id and
            cc.id         = sd.conv_currency_id
          order by
            sd.id desc",WEBPAGE::$userID));

array_unshift($mrow,$extra);

WEBPAGE::makecachefile($mrow,WEBPAGE::$queryCache.'.'.$extra['param']['ts']);
require_once '../app/includes/BS.SCR.pager.inc';

?>