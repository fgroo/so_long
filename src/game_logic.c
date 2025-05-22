// Hilfsfunktion zum Initialisieren der Spielstruktur


#include "../inc/so_long.h"
#include <X11/keysym.h> // Für Tastensymbole wie XK_Escape, XK_w etc.
#include <X11/X.h>      // Für Event-Masken und Event-Typen wie DestroyNotify
#include <stdlib.h>     // Für exit, free, malloc
#include <stdio.h>      // Für printf (Debuggen oder Spielmeldungen)
#include <string.h>     // Für strcmp, strlen etc. (falls benötigt für Map-Parsing)

// Definieren Sie die Größe jedes Tiles (z.B. Wand, Spieler) in Pixeln
#define TILE_SIZE 64

// Struktur für den Spielzustand
typedef struct s_game {
    void    *mlx_ptr;
    void    *win_ptr;
    char    **map;                // Die 2D-Karte
    int     map_width_tiles;      // Kartenbreite in Tiles
    int     map_height_tiles;     // Kartenhöhe in Tiles
    int     player_x_tile;        // Spieler X-Koordinate (Tile-basiert)
    int     player_y_tile;        // Spieler Y-Koordinate (Tile-basiert)
    int     collectibles_total;
    int     collectibles_found;
    int     player_moves;
    void    *img_wall;
    void    *img_path;
    void    *img_player;
    void    *img_collectible;
    void    *img_exit_open;
    void    *img_exit_closed;    // Wenn noch nicht alle Collectibles gesammelt wurden
} t_game;

// Funktionsprototypen (Beispiele)
void    load_assets(t_comps *game);
void    parse_map(t_comps *game, char **map_data); // Um Startpos, Collectibles etc. zu finden
int     render_game_map(t_comps *game, t_map *map);
int 	handle_keypress(int keysym, void *param);
int     cleanup_and_exit(t_comps *game);
// void    initialize_game_struct(t_comps *game_state); // Initialisiert Zeiger auf NULL etc.


// void initialize_game_struct(t_comps *game_state) {
//     game_state->mlx_ptr = NULL;
//     game_state->win_ptr = NULL;
//     game_state->map = NULL;
//     game_state->map_width_tiles = 0;
//     game_state->map_height_tiles = 0;
//     game_state->player_x_tile = -1;
//     game_state->player_y_tile = -1;
//     game_state->collectibles_total = 0;
//     game_state->collectibles_found = 0;
//     game_state->player_moves = 0;
//     game_state->img_wall = NULL;
//     game_state->img_path = NULL;
//     game_state->img_player = NULL;
//     game_state->img_collectible = NULL;
//     game_state->img_exit_open = NULL;
//     game_state->img_exit_closed = NULL;
// }


int cleanup_and_exit(t_comps *game)
{
    if (!game) return (1);

    // Bilder zerstören
    if (game->mlx_ptr) { // Nur wenn mlx_ptr existiert
        if (game->img_player) mlx_destroy_image(game->mlx_ptr, game->img_player);
        if (game->img_wall) mlx_destroy_image(game->mlx_ptr, game->img_wall);
        if (game->img_path) mlx_destroy_image(game->mlx_ptr, game->img_path);
        if (game->img_collectible) mlx_destroy_image(game->mlx_ptr, game->img_collectible);
        if (game->img_exit_open) mlx_destroy_image(game->mlx_ptr, game->img_exit_open);
        if (game->img_exit_closed) mlx_destroy_image(game->mlx_ptr, game->img_exit_closed);

        // Fenster zerstören
        if (game->win_ptr) mlx_destroy_window(game->mlx_ptr, game->win_ptr);
        
        // Display-Verbindung schließen und mlx_ptr freigeben
        // mlx_destroy_display schließt die Verbindung zum X-Server.
        mlx_destroy_display(game->mlx_ptr);
        free(game->mlx_ptr); // Gib die mlx_ptr Struktur selbst frei.
    } 
    // Gib die game-Struktur selbst frei, wenn sie dynamisch allokiert wurde
    // free(game); // Falls game selbst auf dem Heap ist.

    printf("Game exited.\n");
    exit(0); // Programm beenden
    return (0); // Wird nicht erreicht, aber für Konsistenz
}


// Hilfsfunktion zum Laden der Bilder
void load_assets(t_comps *game)
{
    int img_w, img_h; // Dummy-Variablen für Breite und Höhe
    // Ersetze "path/to/your/xpm/" mit den tatsächlichen Pfaden zu deinen XPM-Dateien.
    // Stelle sicher, dass TILE_SIZE mit der Größe deiner XPMs übereinstimmt oder skaliere sie.
    game->img_path = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/path_b.xpm", &img_w, &img_h);
    game->img_wall = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/wall_b.xpm", &img_w, &img_h);
    game->img_player = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/player_b.xpm", &img_w, &img_h);
    game->img_collectible = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/collectible_b.xpm", &img_w, &img_h);
    game->img_exit_open = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/exit_open_b.xpm", &img_w, &img_h);
    game->img_exit_closed = mlx_xpm_file_to_image(game->mlx_ptr, "./textures/exit_close_b.xpm", &img_w, &img_h);

    // Überprüfe, ob alle Bilder erfolgreich geladen wurden
    if (!game->img_path || !game->img_wall || !game->img_player ||
        !game->img_collectible || !game->img_exit_open || !game->img_exit_closed) {
        fprintf(stderr, "Error: Could not load all game assets (XPM images).\n");
        cleanup_and_exit(game); // Versuche, bereits geladene Teile freizugeben und zu beenden
    }
}

// Diese Funktion zeichnet den gesamten aktuellen Spielzustand.
t_game_context	update_chars_and_combine(t_comps *game, t_map *map)
{
	t_game_context	game_ctx;

	map->screen[game->player.x][game->player.y] = 'P';
	map->screen[game->collectible.x][game->collectible.y] = 'C';
	map->screen[game->exit.x][game->exit.y] = 'E';
	game_ctx.comps = game;
	game_ctx.map_data = map;
	return(game_ctx);
}

int render_game_map(t_comps *game, t_map *map)
{
    int x_tile, y_tile;
    void *current_img;

    mlx_clear_window(game->mlx_ptr, game->win_ptr); // Fenster leeren

    for (y_tile = 0; y_tile < map->rows; y_tile++) {
        for (x_tile = 0; x_tile < map->cols; x_tile++) {
            int pixel_x = x_tile * TILE_SIZE;
            int pixel_y = y_tile * TILE_SIZE;
            char tile_type = map->screen[y_tile][x_tile];
            current_img = NULL; // Standardmäßig nichts zeichnen oder Pfadbild
            if (tile_type == '1')
                current_img = game->img_wall;
            else if (tile_type == '3')
                current_img = game->img_path;
            else if (tile_type == 'C')
                current_img = game->img_collectible;
            else if (tile_type == 'E')
			{
                if (game->collectibles_found == 1)
                	current_img = game->img_exit_open;
                else
                	current_img = game->img_exit_closed;
            }
            if (game->img_path)
                 mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_path, pixel_x, pixel_y);
            if (current_img != NULL && current_img != game->img_path) // Zeichne spezifisches Tile über den Pfad
                mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, current_img, pixel_x, pixel_y);
        }
    }

    // Zeichne den Spieler an seiner aktuellen Position (über anderen Tiles)
    if (game->img_player) {
        mlx_put_image_to_window(game->mlx_ptr, game->win_ptr, game->img_player,
                                (game->player.y * 64),
                                (game->player.x * TILE_SIZE));
    }
    // Optional: Spielinformationen wie Züge oder gesammelte Items anzeigen
    // char move_str[50];
    // sprintf(move_str, "Moves: %d Collectibles: %d/%d", game->player_moves, game->collectibles_found, game->collectibles_total);
    // mlx_string_put(game->mlx_ptr, game->win_ptr, 10, 10, 0xFFFFFF, move_str);
    return (0);
}


// Diese Funktion wird aufgerufen, wenn eine Taste gedrückt wird.
int handle_keypress(int keysym, void *param)
{
	t_game_context	*ctx;
    int next_x;
    int next_y;
	char target_tile;

	ctx = (t_game_context *)param;
	next_x = ctx->comps->player.x;
	next_y = ctx->comps->player.y;
    if (keysym == XK_Escape)// XK_Escape anstelle von 65307 für bessere Lesbarkeit
		cleanup_and_exit(ctx->comps);


    // Spielerbewegungen basierend auf Tasten
    if (keysym == XK_w || keysym == XK_Up)    // W oder Pfeil nach oben
        next_x--;
    else if (keysym == XK_a || keysym == XK_Left)  // A oder Pfeil nach links
        next_y--;
    else if (keysym == XK_s || keysym == XK_Down)  // S oder Pfeil nach unten
        next_x++;
    else if (keysym == XK_d || keysym == XK_Right) // D oder Pfeil nach rechts
        next_y++;
    else
        return (0); // Keine relevante Taste gedrückt

    // Überprüfe Kartengrenzen
    if (next_x < 0 || next_x >= ctx->map_data->rows||
        next_y < 0 || next_y >= ctx->map_data->cols)
        return (0); // Außerhalb der Karte

    target_tile = ctx->map_data->screen[next_x][next_y];

    if (target_tile == '1') { // Wand
        return (0); // Bewegung nicht möglich
    }

    // Gültige Bewegung: Spieler aktualisieren
    ctx->comps->player.x = next_x;
    ctx->comps->player.y = next_y;
    ctx->comps->player_moves++;
    printf("Moves: %d\n", ctx->comps->player_moves);


    if (target_tile == 'C') { // Collectible
        ctx->comps->collectibles_found++;
        ctx->map_data->screen[next_x][next_y] = '3'; // Markiere Collectible als eingesammelt (wird zu Pfad)
        printf("Collected: /%d\n", ctx->comps->collectibles_found);
    } else if (target_tile == 'E') { // Ausgang
        if (ctx->comps->collectibles_found == 1) {
            printf("Congratulations! You reached the exit in %d moves!\n", ctx->comps->player_moves);
            cleanup_and_exit(ctx->comps);
        } else {
            printf("Exit is closed! Collect item first (/%d).\n", ctx->comps->collectibles_found);
            // Spieler kann auf das Exit-Feld gehen, aber das Spiel endet nicht
        }
    }

    // Neuzeichnen der Szene nach jeder gültigen Bewegung
    render_game_map(ctx->comps, ctx->map_data);
    return (0);
}


int	main(void)
{
	t_map	gnl;
	t_comps	map_components;
	t_game_context	game_ctx;

	gnl = gnl_engine();
	printf("Bild vor iterativem Floodfill:\n");
	gnl.screen = initialize_map(gnl.colsstring, gnl.cols, gnl.rows);
	map_components = save_map_components(gnl.screen, gnl.cols, gnl.rows);
	printScreenIter(gnl.screen, gnl);
	if (map_components.player.x < 0 || map_components.player.x >= gnl.rows
		|| map_components.player.y < 0 || map_components.player.y >= gnl.cols)
		return (printf("Startpunkt außerhalb der Grenzen!\n"), 1);
	gnl.screen = rdy_for_floodfill(gnl.screen, map_components); // Wichtig: oldColor muss die Farbe des Startpixels sein!
	floodfilliterative(gnl.screen, map_components.player, gnl);
	game_ctx = update_chars_and_combine(&map_components, &gnl);
	printf("Bild nach iterativem Floodfill:\n");
	printScreenIter(gnl.screen, gnl);
	
	
	// initialize_game_struct(&map_components);

	map_components.mlx_ptr = mlx_init(); // Initialisiere MiniLibX
	if (!map_components.mlx_ptr)
		return (fprintf(stderr, "Error: mlx_init() failed.\n"), 1);

	// Lade Grafiken (XPM-Dateien)
	load_assets(&map_components);

	// Erstelle das Fenster
	map_components.win_ptr = mlx_new_window(map_components.mlx_ptr, (gnl.cols) * 65, gnl.rows * 64, "so_long Game");
	if (!map_components.win_ptr)
	{
		fprintf(stderr, "Error: mlx_new_window() failed.\n");
		cleanup_and_exit(&map_components); // Versuche, Ressourcen freizugeben
		return (1);
	}

	// Registriere Hook-Funktionen
	// mlx_key_hook wird für Tastenanschläge (KeyRelease-Event) verwendet
	mlx_key_hook(map_components.win_ptr, handle_keypress, &game_ctx);

	// Hook für das Schließen des Fensters über den [X]-Button des Window Managers
	// DestroyNotify ist Event-Typ 17. StructureNotifyMask ist (1L<<17).
	// mlx_loop behandelt WM_DELETE_WINDOW intern und ruft den DestroyNotify Hook auf, falls gesetzt.
	mlx_hook(map_components.win_ptr, DestroyNotify, StructureNotifyMask, cleanup_and_exit, &map_components);

	// Initiales Rendern der Karte
	render_game_map(&map_components, &gnl);

	// Starte die Haupt-Event-Schleife von MiniLibX
	mlx_loop(map_components.mlx_ptr);

	// cleanup_and_exit wird normalerweise von einem Hook oder ESC aufgerufen.
	// Falls mlx_loop aus irgendeinem Grund normal endet, hier aufräumen.
	//cleanup_and_exit(&game_state); 
	while (gnl.rows--)
			(free(gnl.colsstring[gnl.rows]), free(gnl.screen[gnl.rows]));
	return (free(gnl.screen), 0);
}
