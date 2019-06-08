grammar = """
    query: select special_rule select_rule from table_rule join_rule where_rule groupby_rule having_rule orderby_rule limit_rule 

    special_rule: DISTINCT |
    DISTINCT: "DISTINCT"

    select: SELECT
    SELECT: "SELECT"
    
    from: FROM
    FROM: "FROM"
    
    select_rule: /["*"]/ 
                | attribute_rule 
    
    attribute_rule : attribute_type "," attribute_rule  
                |  attribute_type 
                | attribute_name_alias
				| agg openbraces attribute_type closebraces as aliasname 
                | agg openbraces attribute_type closebraces 
				| agg openbraces attribute_type closebraces as aliasname "," attribute_rule 
                | agg openbraces attribute_type closebraces "," attribute_rule 
    
    as: AS
    AS: "AS"
    MAX: "MAX"
    AVG: "AVG"
    COUNT: "COUNT"
    MIN: "MIN"
    SUM: "SUM"
    
    agg: SUM 
        | COUNT 
        | MAX 
        | MIN 
        | AVG

    attribute_name_alias : attribute_type as aliasname 
                            | attribute_type as aliasname "," attribute_rule
    
    openbraces: /["("]/
    
    closebraces: /[")"]/
    
    table_rule : tablename "," table_rule 
                | tablename 
                | tablename as aliasname 
                | tablename as identifier "," table_rule
    
    join_rule : join table_rule on aliasname "." attribute_type eq aliasname "." attribute_type join_rule | 
    
    join: INNER JOIN 
            | LEFT JOIN 
            | RIGHT JOIN 
            | FULL JOIN
    
    INNER: "INNER"
    LEFT: "LEFT"
    RIGHT: "RIGHT"
    FULL: "FULL"
    JOIN: "JOIN"
    on: ON
    ON: "ON"
    
    eq: /["="]/
    
    where_rule : where condition_rule | where not condition_rule | 
    where: WHERE
    WHERE: "WHERE"
    not: "NOT"

    condition_rule : flat_condition and condition_rule | flat_condition or condition_rule | flat_condition
    and: AND
    AND: "AND"
    or: OR
    OR: "OR"
    
    flat_condition : identifier comparison_operator value 
                | aliasname "." identifier comparison_operator aliasname "." identifier
                | aliasname "." identifier comparison_operator value 
                | attribute_type between value and value 
				| openbraces attribute_type between value and value closebraces 
                | attribute_type like pattern 
                | attribute_type in openbraces attribute_list closebraces 
                | attribute_type not in openbraces attribute_list closebraces 
                | attribute_type in openbraces query closebraces 
				| attribute_type not in openbraces query closebraces 
                | exists openbraces query closebraces
                | identifier comparison_operator openbraces query closebraces 
				| aliasname "." identifier comparison_operator openbraces query closebraces
				| identifier comparison_operator any_all_some openbraces query closebraces 
				| aliasname "." identifier comparison_operator any_all_some openbraces query closebraces
        
    between: BETWEEN
    BETWEEN: "BETWEEN"
    like: LIKE
    LIKE: "LIKE"
    comparison_operator: /[">"]/ | /["<"]/ | /[">"]["="]/ | /["<"]["="]/ | /["="]/ | /["!"]["="]/  

    in: IN
    IN: "IN"
    exists: EXISTS
    EXISTS: "EXISTS"

    attribute_list : attribute_type "," attribute_list | attribute_type  
        
    any_all_some : ANY | ALL |SOME 
    ANY: "ANY"
    SOME: "SOME"
    ALL: "ALL"
    
    orderby_rule : order by order_expr | 
    
    order: ORDER
    ORDER: "ORDER"
    by: BY
    BY: "BY"

    order_expr : attribute_type "," order_expr 
            | attribute_type asc 
            | attribute_type desc
            | attribute_type asc "," order_expr 
            | attribute_type desc "," order_expr 
            | attribute_type
    asc: ASC
    ASC: "ASC"
    desc: DESC
    DESC: "DESC"

    limit_rule : limit limit_expr | 
    
    limit: LIMIT
    LIMIT: "LIMIT"

    limit_expr : digit | digit offset digit 
    
    offset: OFFSET
    OFFSET: "OFFSET"

    groupby_rule : group by groupby_expr |
    group: GROUP
    GROUP: "GROUP"

    groupby_expr : attribute_type "," groupby_expr | attribute_type 

    having_rule : having having_expr | 
    having: HAVING
    HAVING: "HAVING"


    having_expr : flat_condition | agg openbraces attribute_type closebraces having_cond
         

    having_cond : comparison_operator value 
            | between value and value 
            | like pattern
            | in openbraces attribute_list closebraces 
            | not in openbraces attribute_list closebraces 
            | in openbraces query closebraces 
            | not in openbraces query closebraces 
            | exists openbraces query closebraces 
            | comparison_operator openbraces query closebraces 
            | comparison_operator any_all_some openbraces query closebraces 

    value: /[0-9]+/ | /[a-zA-Z][a-zA-Z0-9]+/ | /[0-9]+["."][0-9]+/ | stringconst
    stringconst: /["\""][a-zA-Z0-9@#._&$!]+["\""]/
    pattern: /["%"]?[_a-z]+["%"]?/
    digit: /[0-9]+/
    tablename: /[a-zA-Z][a-zA-Z0-9]+/
    aliasname: /[a-zA-Z][a-zA-Z0-9]+/
    attribute_type : aliasname "." identifier | identifier 
    identifier: /[a-zA-Z][a-zA-Z0-9]+/
    
    %import common.WS
    %ignore WS
"""
