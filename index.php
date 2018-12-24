

<!DOCTYPE html>
<html>
    <head>
            <title>Search Engine</title>

            <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css">
            <link rel="stylesheet" type="text/css" href="font-awesome/css/font-awesome.min.css">
            <link rel="stylesheet" type="text/css" href="css/styles.css">

            <link rel="shortcut icon" href="favicon.png">
    </head>

    <body>
    
    <main role="main" class="container">
        
        <div class="text-center">
            <img src="logo.png" class="logo">
        </div>
            
      <div class="d-flex align-items-center p-3 my-3 text-white-50 bg-purple rounded shadow-sm">
        
            <form class="form-inline" method="post" action="">
            
                <button type="submit" name="search-submit" class="btn btn-primary my-2">Search</button>
                <div class="form-group mx-sm-3 my-2">
                  <input type="text" name="query" class="" id="query" placeholder="Search Here">
                </div>

            </form>
          
      </div>

      <div class="my-3 p-3 bg-white rounded shadow-sm">
        <h6 class="border-bottom border-gray pb-2 mb-0">Results</h6>
        
        
        
        
        <?php
        
            if(isset($_POST['search-submit']))
            {
                $query = $_POST['query'];
                
                //exec("PHPtest.bin ".$query, $output);
                // subject1-:-How to kill myself-;;-subject2-:-how to drink water-;;-subject-:-how to be a snake-;;-
                
                $results = explode("-;;-", $query);
                
                for($i=0; $i<sizeof($results)-1; $i++)
                {
                    $results[$i] = explode("-:-", $results[$i]);;
                }
                
                for($i=0; $i<sizeof($results)-1; $i++)
                {
                    echo '<div class="media text-muted pt-3">
                            <img data-src="holder.js/32x32?theme=thumb&bg=6f42c1&fg=6f42c1&size=1" alt="" class="mr-2 rounded">
                            <p class="media-body pb-3 mb-0 small lh-125 border-bottom border-gray">
                              <strong class="d-block text-gray-dark">'.$results[$i][0].'</strong><br>
                              '.$results[$i][1].'
                            </p>
                          </div>';
                }
            }
        
        ?>
        
      </div>
    </main>
        
        
        
           
    </body>
</html>