<!DOCTYPE HTML>
<html>
	<head>
		<title>SMART BUILDING</title>
		<meta charset="utf-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1" />
		<link rel="stylesheet" href="assets/css/main.css" />
		<script src="assets/js/raphael-2.1.4.min.js"></script>
        <script src="assets/js/justgage.js"></script>

		
	</head>
	<body bgcolor="#80dfff">
			<div class="heading">
				<span id="hfont"> SMART BUILDING</span>
			</div>
			<?php
                $cont = file_get_contents("http://88.88.88.20/");
				$contarr = split ("\,", $cont);
                $temp = $contarr[0];
				$light = $contarr[1];		
				$co2 = $contarr[2];
				$cfactor = $contarr[3];
                                                
			<div style="width:100%;"> <!-- Main Div -->
				<div style="float:left; width:50%;">
					 <div class="sensor">TEMPERATURE</div>
			<div class="data" > echo $temp</div>
			<div class="sensor">LIGHT</div>
			<div class="data" > echo $light</div>
			<div class="sensor">Carbon Dioxide</div>
			<div class="data" > echo $co2</div>
					</div>
							<div style="float:left; width:40%; margin-left:10px;">
							<div id="bigfella" style="width:400px; height:320px; padding-top:70px; padding-left: 80px;"></div>
							    
				<div>
			
			<script>
	var g = new JustGage({
    id: "bigfella",
    value: $cfactor,
    min: 0,
    max: 3,
	title: "Comfort Factor"
  });
</script>
			?>
			
			
			
	</body>
</html>