<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>

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
	width: 300px;
	height: 70px;
	-moz-border-radius:15px;
	-webkit-border-radius:15px;
	-o-border-radius:15px;
	-ms-border-radius:15px;
	-khtml-border-radius:15px;
	border-radius: 15px;
	font: normal 45px/45px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	color: #ccc;
	text-shadow: 0 0;
	-moz-box-shadow: 0 0 5px #000000;
	-webkit-box-shadow: 0 0 5px #000000;
	box-shadow: 0 0 5px #000000;
	border: none;
	padding-left: 10px;
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
	display: none;
}

#sponsorshipAdminExpenseCheckbox + label {
	display: inline-block;
	width: 25px;
	height: 25px;
	background-color: white;
	-moz-border-radius:5px;
	-webkit-border-radius:5px;
	-o-border-radius:5px;
	-ms-border-radius:5px;
	-khtml-border-radius:5px;
	border-radius: 5px;
	behavior: url(border-radius.htc);
	text-shadow: 0 0;
	-moz-box-shadow: 0 0 5px #000000;
	-webkit-box-shadow: 0 0 5px #000000;
	box-shadow: 0 0 5px #000000;
	border: none;
	float: left;
	margin: 10px 10px 0 100px;
	position: relative;
}

#sponsorshipAdminExpenseCheckbox:checked + label {
	display: inline-block;
	width: 25px;
	height: 25px;
	background-color: #e0af00;
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
	font: 10px "Helvetica Neue", Arial, Helvetica, Geneva, sans-serif;
	text-transform: uppercase;
	margin-top: 10px;
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
}

</style>


</head>

<body>
<div class="box1"></div>
<div class="sponsorshipContainer">
<!--	<div class="sponsorshipExplanation">
	<p>Contribute to funding the loand request of this group of five entrepreneur and follow their progress online as they repay their loans.</p>
	<a href="">Tell me more.</a>
	</div>
-->	
	<div class="sponsorshipPhoto" >
                <div class="photoDescription">
			<p class="sponsorshipGroupName">{bde_name}</p>
			<p class="sponsorshipGroupLocation">Location: {branch} - {country}</p>
		</div>
	</div>
	
	<div class="sponsorshipInfo">
		<p class="sponsorshipGoal">Funding Goal</p>
		<p class="sponsorshipAmount">${amount}</p>
		<div class="progressBarContainer">	
			<div class="progressBar" style="width: {progress}%">
				<p class="progressBarAmount">${donations}</p>
			</div>
		</div>
		<p class="sponsorshipDays">{time_left} day(s) left</p>
		<a href="index.pub.php?scr_name=PB.SCR.donateOne&lang=en&id={master_id}" class="sponsorshipButton" title="Fund this group">Fund This Group<small>${min_donation} minimum donation</small></a>
		<p class="sponsorshipNext"><a href="index.pub.php?scr_name=PB.SCR.donateHome&lang=en&id={master_id}&prev=1" class="sponsorshipNext">previous group</a> | <a href="index.pub.php?scr_name=PB.SCR.donateHome&lang=en&id={master_id}&next=1" class="sponsorshipNext">next group</a></p>
		<br />
		<a class="sponsorshipNext" href="http://esperanza.org/home/how-can-i-help/412-sponsor-a-bank-of-esperanza.html" target="_parent">Tell me more.</a>
	</div>	
</div>	
	
</body>
</html>

