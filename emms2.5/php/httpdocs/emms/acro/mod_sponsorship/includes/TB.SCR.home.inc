<?
$tpl->setVariable('javascript',          "<SCRIPT type='text/javascript' src='CS.functions.js'></SCRIPT>");
$tpl->setVariable('welcome',             sprintf($welcome, $_SESSION['_authsession']['data']['first']));
$tpl->setVariable('shortcuts',           WEBPAGE::$gt['quick_jumps']);
$tpl->setVariable('editProfile',         WEBPAGE::$gt['SP.SCR.editProfile']);
$tpl->setVariable('changePassword',      WEBPAGE::$gt['SP.SCR.changePassword']);
$tpl->setVariable('financialSummary',    WEBPAGE::$gt['SP.SCR.financialSummary']);
$tpl->setVariable('donationsHistory',    WEBPAGE::$gt['SP.SCR.donationsHistory']);
$tpl->setVariable('portfolioSummary',    WEBPAGE::$gt['SP.SCR.portfolioSummary']);
$tpl->setVariable('clientSummary',       WEBPAGE::$gt['SP.SCR.clientSummary']);
$tpl->setVariable('loanDefaults',        WEBPAGE::$gt['tblLoanTypes.id']);
$tpl->setVariable('disbursmentHistory',  WEBPAGE::$gt['SP.SCR.disbursmentHistory']);
$tpl->setVariable('businessSupport',     WEBPAGE::$gt['SP.SCR.businessSupport']);
$tpl->setVariable('browseLoans',         WEBPAGE::$gt['SP.SCR.browseLoans']);
$tpl->setVariable('portfolioAtRisk',     WEBPAGE::$gt['SP.SCR.portfolioAtRisk']);
$tpl->setVariable('writeOffHistory',     WEBPAGE::$gt['SP.SCR.writeOffHistory']);
$tpl->setVariable('browseGroups',        WEBPAGE::$gt['SP.SCR.browseGroups']);
$tpl->setVariable('browseClients',       WEBPAGE::$gt['SP.SCR.browseClients']);
$tpl->setVariable('accessLog',           WEBPAGE::$gt['SP.SCR.accessLog']);
$tpl->setVariable('logout',              WEBPAGE::$gt['logout']);
$tpl->setVariable('mktime',              mktime());
$tpl->setVariable('lang',                WEBPAGE::$lang);
?>


