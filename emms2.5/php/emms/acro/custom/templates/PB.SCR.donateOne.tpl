<!DOCTYPE HTML PUBLIC "-//W3c//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head><META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-3">
<title>{org_name} - Sponsor a Bank Of Esperanza</title>

<style type="text/css">
.box1 {
	width:0;
	height:0;
	display:none;
	border-radius:1px;
	behavior: url(border-radius.htc);
}

body {
	background-color: #f8f8f8;
}

.sponsorshipContainer{
	width: 960px;
}

/* The Progress Bar
---------------------------------*/
.progressBar
{
	background:#1e557a;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius:15px;
	behavior: url(border-radius.htc);
	height:25px;
	min-width: 8%;
	overflow: visible;
	font: 20px/1.1471em "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	position: relative;
}

.progressBarContainer
{
	width: 100%;
	background:#999;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius:15px;
	behavior: url(border-radius.htc);
	height:25px;
	position: relative;
}

/* The Information
---------------------------------------*/

.progressBarAmount {
	text-align: right;
	margin: 15px 15px 0 25px;
	color: #fff;
	
	/* any IE */
	margin-top:0;
}

.sponsorshipGoal {
	margin: 0 10px 0 0;
	font: bold 14px/14px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	text-align: right;
}

.sponsorshipAmount {
	margin: 0 5px 10px 0;
	font: 45px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	font-weight: bold;
	line-height: 45px;
	text-align: right;
}

.sponsorshipInfoCheckout .sponsorshipAmount
 {
 	font-size: 36px;
 	color: #1e557a;
 }

.sponsorshipDays {
	margin: 5px 10px 0 0;
	font: bold 14px/14px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	text-align: right;
}

.sponsorshipNext {
	font: bold 10px/14px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	text-align: right;
	color: #999;
	margin-left: 5px;
	margin-right: 5px;
	text-decoration: none;
}

.sponsorshipGroupName {
	font: 14px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	font-weight: bold;
	text-align: left;
	margin: 0;
}

.sponsorshipGroupLocation {
	font: 12px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	text-align: left;
	margin: 0;
}

.sponsorshipExplanation, .sponsorshipExplanation p {
	font: 15px/22px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	margin: 0;
}

.sponsorshipExplanation a {
	display: block;
	margin-top: 15px;
	color: #999;
	font-size: 12px;
}

/* The Button
-----------------------------*/

.sponsorshipButtonContainer {
	text-align: right;
}

.sponsorshipButton  {
	width: 180px;
	height: 40px;
	display: block;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius:15px;
	behavior: url(border-radius.htc);
	background-color: #e0af00;
	text-decoration: none;
	color: #1e557a;
	text-transform: uppercase;
	font: bold 18px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	padding: 15px;
	margin: 10px 0 25px 120px;
	text-align: center;
	float: right;
	position: relative;
	
	/* any IE */
	margin: 10px 0 25px 80px;
}

.sponsorshipAmountInputContainer {
	width: 300px;
	height: 70px;
	-moz-border-radius: 15px;
	-webkit-border-radius: 15px;
	-o-border-radius: 15px;
	-khtml-border-radius: 15px;
	-ms-border-radius: 15px;
	border-radius: 15px;
	behavior: url(border-radius.htc);
	background-color: white;
	position:relative;
	margin: 10px 0 0 0;
	padding-left:10px;
	border: 1px #000 solid;
}

.sponsorshipButton:hover {
	background-color: #1e557a;
	color: white;
}
/*
.sponsorshipButton:active {
	color: #e0af00;
	background-color: #1e557a;
}*/

.sponsorshipButton small {
	display: block;
	font-size: 13px;
}

/* The Image
-------------------------------*/

.sponsorshipPhoto, .sponsorshipPhoto img, .sponsorshipPhotoCheckout img {
	width: 100%;
	height: 100%;
	z-index: 1;
	margin: 0;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius: 15px;
	behavior: url(border-radius.htc);
	position: relative;
}

.photoDescription {
	margin-top: 235px;
	padding: 5px 0 0 15px;
	background-color: rgba(29, 84, 122, 0.9);
	opacity: 0.73;
	height: 35px;
	color: white;
	z-index:10;
	
	/* any IE */
	background-color: rgb(29, 84, 122);
        opacity: 0.73;
}

/* The forms
-----------------------------------*/

#sponsorshipAmountInput  {
	width: 288px;
	height: 50px;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius: 15px;
	behavior: url(border-raduis.htc);
	font: normal 45px/45px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	color: #ccc;
	text-shadow: 0 0;
	-moz-box-shadow: 0 0 5px #000000;
	-webkit-box-shadow: 0 0 5px #000000;
	box-shadow: 0 0 5px #000000;
	border: none;
	padding: 5px 0 5px 10px;
	margin: 10px 0 0 0;
	position: relative;
}

#sponsorshipAmountInput:active, #sponsorshipAmountInput:focus, sponsorshipAmountInput:after {
	color: black;
	font-weight: bold;
}

.sponsorshipCheckoutAdminexpenseExplanation {
	font: 12px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
}

#sponsorshipAdminExpenseCheckbox {
	display:block;
	float: left;
	margin: 3px 10px 10px 0;
}


.sponsorshipCheckoutAdminexpenseExplanation h2, .sponsorshipCheckoutAdminexpenseExplanation h3, .sponsorshipCheckoutAdminexpenseExplanation h4 {
	margin: 0 10px 0 0;
	font: bold 14px/14px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	text-align: left;
}

.sponsorshipCheckoutAdminexpenseExplanation h4 {
	margin: 25px 10px 0 0;
}
 
.sponsorship_tip {
	font: 12px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	margin: 10px 0;
	display:block;
	margin: 10px 0 0 100px;
}

/* The three content blocks
-----------------------------------*/
.sponsorshipExplanation, .sponsorshipPhoto, .sponsorshipPhotoCheckout, .sponsorshipInfo, .sponsorshipCheckoutAdminexpense {
	display: block;
	position: relative;
	float: left;
	margin-right: 20px;
}

.sponsorshipInfoCheckout
 {
	display: block;
	position: relative;
	float: left;
}

.sponsorshipExplanation {
	width: 150px;
}

.sponsorshipPhoto {
	width: 600px;
	height: 287px;
	background:url({image});
	margin:0;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius: 15px;
	border-radius:15px;
	behavior: url(border-radius.htc);
	z-index:-1;
}

.sponsorshipInfo {
	width: 300px;
	margin-left: 20px;
}

.sponsorshipInfoCheckout, .sponsorshipPhotoCheckout, .sponsorshipCheckoutAdminexpense {
	width: 300px;
	height: 287px;
	margin-left: 15px;
}

</style>

<script language="JavaScript">

function submitForm()
{
  if(document.donateForm.onsubmit())
    document.donateForm.submit();
}

function verifyAmount()
{
  minD = {min_donation};
  num = document.donateForm.donation.value;
  num = num.toString().replace(/\$|\,/g,'');
  if(isNaN(num)||(num < minD))
  {
    document.donateForm.donation.value = "${min_donation} minimum";
    return false;  
  }  
  if(num > {fpending})
  {
    document.donateForm.donation.value = "${pending} max";
    return false;  
  }  
  sign = (num == (num = Math.abs(num)));
  num = Math.floor(num*100+0.50000000001);
  cents = num%100;
  num = Math.floor(num/100).toString();
  if(cents < 10)
  cents = "0" + cents;
  for (var i=0; i<Math.floor((num.length-(1+i))/3); i++)
    num = num.substring(0,num.length - (4*i+3)) + ',' + num.substring(num.length - (4*i+3));
  document.donateForm.donation.value = num + '.' + cents; 
  return true; //window.alert(document.donateForm.donation.value);
}

function formatCurrencyBAK()
{
  minD = {min_donation};
  num = document.donateForm.donation.value;
  num = num.toString().replace(/\$|\,/g,'');
  if(isNaN(num)||(num < minD))
    num = minD;
  if(num > {fpending})
    num = {fpending};
  sign = (num == (num = Math.abs(num)));
  num = Math.floor(num*100+0.50000000001);
  cents = num%100;
  num = Math.floor(num/100).toString();
  if(cents < 10)
  cents = "0" + cents;
  for (var i=0; i<Math.floor((num.length-(1+i))/3); i++)
    num = num.substring(0,num.length - (4*i+3)) + ',' + num.substring(num.length - (4*i+3));
  document.donateForm.donation.value = num + '.' + cents; //window.alert(document.donateForm.donation.value);
}
</script>

</head>
<body>
<div class="box1"></div>
<div class="sponsorshipContainer">
	<div class="sponsorshipPhoto">
		<div class="photoDescription">
			<p class="sponsorshipGroupName">{bde_name}</p>
			<p class="sponsorshipGroupLocation">Location: {branch} - {country}</p>
		</div>
	</div>
	
	<div class="sponsorshipInfoCheckout">
        <form name="donateForm" onsubmit="return verifyAmount()" target=_top method=post action='index.pub.php?scr_name=PB.SCR.donateTwo&lang={lang}&id={master_id}'>
		<p class="sponsorshipGoal">left to raise</p>
		<p class="sponsorshipAmount">${pending}</p>
		<p class="sponsorshipGoal">of ${amount} funding Goal</p>
                <input type="hidden" name="master_id" value="{master_id}">
		<input type="text" name="donation" id="sponsorshipAmountInput" value="${min_donation} minimum" onClick="this.value=''" />
		<p class="sponsorship_tip"><input type="checkbox" checked name="tip" id="sponsorshipAdminExpenseCheckbox" />
		Add a {admin_tip}% to help with<br/>administrative expenses</p>
		<a href="javascript:submitForm();" class="sponsorshipButton" title="Fund this group">Fund This Group<small>Check out with paypal</small></a>
        </form>
	</div>	
</div>	
</body>
</html>