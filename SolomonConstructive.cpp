SolomonConstructive
solve(customers, depot, planning horizon, strategy)
{
    for i from 0 to planning horizon-1
        create route with depot
        insert seed customer into route according to strategy

    while customers > 0
        for each customer
            for each route
                for each position in route
                    calculate c1
                    if c1 is min:
                        if inserting customer is feasible:
                            set c1 as new min
                            save optimal position, optimal route of customer

        if at least 1 customer is feasible:
            for each customer:
                calculate c2 with c1, optimal position and optimal route of customer
                if c2 is min:
                    save optimal route, optimal customer
        
            insert optimal customer into optimal route at optimal position
            remove customer from customers
        
        else:
            break
    return unrouted customers and routes
}

solve_optional(customers, routes)
{
    while customers > 0
        for each customer
            for each route
                for each position in route
                    calculate c1
                    if c1 is min:
                        if inserting customer is feasible:
                            set c1 as new min
                            save optimal position of route of customer

            if at least 1 route is feasible:
                for each route:
                    calculate c2 with c1, optimal position of route of customer
                    if c2 is min:
                        save optimal route
            
                insert customer into optimal route at optimal position
                remove customer from customers
        
            else:
                break
    return unrouted customers and routes            
}

sequential_best_insert(customers, route)
{
    for each customer
        for each position in route
            calculate c1
            if c1 is min:
                if inserting customer is feasible:
                    set c1 as new min
                    save optimal position of route of customer

        insert customer into route at optimal position that is feasible

    return unrouted customers and routes            
}

TSP3Opt(best route)
{
    calculate best route infeasibility
    do
        set improving = false
        flip three segments of best route such that ABC -> ACB
        set candidate as route ACB
        if candidate length and candidate infeasibility does not increase:
            if either candidate length or candidate infeasibility improves:
                set candidate as best route
                improving = true
    while improving == true

    return best route
}

VNS(solution, customers, stopping criterion)
{
    while stopping criterion is not met:
        set candidate as solution
        for i from 1 to 17:
            i from 1 to 8:
                select 2 routes at random
                crossExchange(route 1, route 2)
                TSP3Opt(route 1)
                TSP3Opt(route 2)
                check feasibility of route 1 and route 2
            i from 9 to 12:
                select 1 route at random
                optExchange1(route, customers)
                TSP3Opt(route)
                check feasiblity of route
            i from 13 to 17：
                select 1 route at random
                optExchange2(route, customers)
                sequentialBestInsert(route, customers)
                check feasiblity of route
        
            if feasible:
                solution = acceptanceDecision(candidate, solution)
    return solution
}

acceptanceDecision(candidate, solution)
{
    if candidate value > solution value:
        return candidate
    else if candidate value == solution value:
        if candidate length < solution value:
            return candidate
        else if max iterations reached without improvement:
            if candidate length > solution length by less than 5%:
                return candidate

    return solution
}

crossExchange(route1, route2, neighborhood)
{
    select 1 segment from route1 and route2 at random
    swap segments in both routes
    return route1 and route2
}

optExchange1(route, customers, neighborhood)
{
    if neighborhood is 9:
        set p = 0
        set q = 1
    if neighborhood is 10:
        set p = 1
        set q = 1
    if neighborhood is 11:
        set p = 2
        set q = 1
    if neighborhood is 12:
        set p = 0
        set q = 2
    
    p = min(p, number of optional customers in route)
    q = min(q, number of unplanned customers)
    
    select a position from route between 0 and length of route - p at random
    for i from 1 to p:
        delete optional customer found at position
        append deleted customer into customers at last position

    for i from 1 to q:
        insert optional customer at position from front of customers

    return unrouted customers and route
}

optExchange2(route, customers, neighborhood)
{
    set p = min(neighborhood - 12, number of optional customers in route)
    select a position from route between 0 and length of route - p at random
    for i from 1 to p:
        delete optional customer found at position

    return unrouted customers and route
}